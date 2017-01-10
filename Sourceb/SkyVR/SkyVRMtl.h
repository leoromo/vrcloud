// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>

/**
 * 
 */
class SKYVR_API SkyVRMtl
{
public:
	SkyVRMtl();
	SkyVRMtl(std::string name_, std::string diffuseMap_);
	~SkyVRMtl();

	std::string name;
	FVector ambient;
	FVector diffuse;
	FVector specular;
	float d;
	float illumination;
	std::string diffuseMap;
};
