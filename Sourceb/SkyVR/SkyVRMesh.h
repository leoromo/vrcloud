// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>
#include "SkyVRMtl.h"

/**
 * 
 */
class SKYVR_API SkyVRMesh
{
public:
	SkyVRMesh();
	~SkyVRMesh();

	bool LoadFromFile(const std::string filename);

	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<int32> indices;
	std::string mtlFile;
	std::string mtlName;
	SkyVRMtl mtl;
};
