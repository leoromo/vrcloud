// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include <string>

#include "SkyVRModelActor.generated.h"


UCLASS()
class SKYVR_API ASkyVRModelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyVRModelActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Path to the cloud file
	UPROPERTY(EditAnywhere)
	FString filename;

	// Path to the material base
	UPROPERTY(EditAnywhere)
	UMaterialInterface *parentMaterial;

private:
	UProceduralMeshComponent *procMesh;

	bool CreateProceduralMeshComponent();
	UTexture* CreateTextureFromFile(std::string texfile);
	
};
