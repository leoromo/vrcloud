// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PointCloud.h"
#include "CloudLoader.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKYCRAFT_API UCloudLoader : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCloudLoader();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Path to the cloud file
	UPROPERTY(EditAnywhere)
		FString filename;

	// Handle to the main character
	UPROPERTY(EditAnywhere)
	class AActor* player;

	// Instantiable object
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> VoxelClass;

	// Voxel size
	UPROPERTY(EditAnywhere)
		float voxelSize;

	UPROPERTY(EditAnywhere)
	class UMaterial* parentMaterial;

	void LoadCloud();
	void InstantiateCloud();

	PointCloud pc;	
};
