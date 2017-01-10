// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyVR.h"
#include "SkyVRModelActor.h"
#include "SkyVRMesh.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"

// Sets default values
ASkyVRModelActor::ASkyVRModelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LoadedObjMesh"));
}

// Called when the game starts or when spawned
void ASkyVRModelActor::BeginPlay()
{
	Super::BeginPlay();
	CreateProceduralMeshComponent();
}

// Called every frame
void ASkyVRModelActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool ASkyVRModelActor::CreateProceduralMeshComponent()
{
	std::string file(TCHAR_TO_UTF8(*filename));
	TSharedPtr<SkyVRMesh> mesh(new SkyVRMesh());
	if (!mesh->LoadFromFile(file))
	{
		UE_LOG(LogTemp, Warning, TEXT("Error opening the obj file"));
		return false;
	}

	// Create procedural mesh
	procMesh->CreateMeshSection(1, mesh->vertices, mesh->indices, mesh->normals, mesh->uvs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	//procMesh->CreateMeshSection(1, mesh->vertices, mesh->indices, TArray<FVector>(), mesh->uvs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	//procMesh->CreateMeshSection(1, mesh->vertices, mesh->indices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	if (procMesh)
	{
		// Create material
		UMaterialInstanceDynamic* dMaterial = UMaterialInstanceDynamic::Create(parentMaterial, this);
		dMaterial->SetTextureParameterValue("DiffuseMap", CreateTextureFromFile(mesh->mtl.diffuseMap));
		procMesh->SetMaterial(1, dMaterial);
		
		// Scale to change from m to cm (UE uses cm)
		RootComponent->SetWorldScale3D(FVector(1000, 1000, 1000));
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Error creating procedural mesh"));
	return false;
}

UTexture* ASkyVRModelActor::CreateTextureFromFile(std::string file)
{
	// Represents the entire file in memory.
	TArray<uint8> rawFileData;

	if (FFileHelper::LoadFileToArray(rawFileData, UTF8_TO_TCHAR(file.c_str())))
	{
		IImageWrapperModule& imageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		IImageWrapperPtr imageWrapper = imageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		if (imageWrapper.IsValid() && imageWrapper->SetCompressed(rawFileData.GetData(), rawFileData.Num()))
		{
			const TArray<uint8>* uncompressedBGRA = NULL;
			if (imageWrapper->GetRaw(ERGBFormat::BGRA, 8, uncompressedBGRA))
			{
				// Create the UTexture for rendering
				UTexture2D* texture = UTexture2D::CreateTransient(imageWrapper->GetWidth(), imageWrapper->GetHeight(), PF_B8G8R8A8);

				// Fill in the source data from the file
				void* textureData = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(textureData, uncompressedBGRA->GetData(), uncompressedBGRA->Num());
				texture->PlatformData->Mips[0].BulkData.Unlock();

				// Update the rendering resource from data.
				texture->UpdateResource();
				return texture;
			}
		}
	}
	return NULL;
}