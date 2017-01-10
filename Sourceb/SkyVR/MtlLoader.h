// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkyVRMtl.h"
#include <unordered_map>
/**
 * 
 */
class SKYVR_API MtlLoader
{
public:
	MtlLoader();
	~MtlLoader();

	bool LoadFromFile(std::string filename);
	SkyVRMtl LoadFromFile(std::string filename, std::string mtlName);

private:
	std::unordered_map<std::string, SkyVRMtl> mtlMap;
	bool fileRead;

	bool LoadMaterialMap(std::string filename);
};
