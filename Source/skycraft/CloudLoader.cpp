// Fill out your copyright notice in the Description page of Project Settings.

#include "skycraft.h"
#include "CloudLoader.h"

// Sets default values for this component's properties
UCloudLoader::UCloudLoader()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCloudLoader::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LoadCloud();
	InstantiateCloud();
}


// Called every frame
void UCloudLoader::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UCloudLoader::LoadCloud()
{
	std::string file(TCHAR_TO_UTF8(*filename));
	pc.ReadFromFile(file);
	pc.Voxelize(voxelSize);
	pc.DeMean();
}

void UCloudLoader::InstantiateCloud()
{
	if (VoxelClass != NULL)
	{
		//Spawn voxels
		UWorld* world = GetWorld();
		const FRotator spawnRot(0, 0, 0);
		for (int i = 0; i < pc.points.size(); ++i)
		{
			Point p = pc.points[i];
			const FVector actorLoc = player->GetActorLocation();
			const FVector spawnLoc = FVector(p.y * 100, p.x * 100, p.z * 100);
			AActor* voxel = world->SpawnActor<AActor>(VoxelClass, spawnLoc, spawnRot);
			voxel->SetActorScale3D(FVector(voxelSize, voxelSize, voxelSize));
			TArray<UStaticMeshComponent*> comps;
			voxel->GetComponents(comps);
			for (auto StaticMeshComponent : comps)
			{
				UMaterialInstanceDynamic* newMat = UMaterialInstanceDynamic::Create(this->parentMaterial, StaticMeshComponent);
				FLinearColor rgb = FLinearColor(p.r / 255.0, p.g / 255.0, p.b / 255.0);
				newMat->SetVectorParameterValue(FName(TEXT("Color")), rgb);
				StaticMeshComponent->SetMaterial(0, newMat);
			}
		}
	}
}
