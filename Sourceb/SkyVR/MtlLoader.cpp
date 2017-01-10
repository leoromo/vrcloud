// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyVR.h"
#include "MtlLoader.h"
#include <fstream>
#include <sstream>
#include <Regex.h>

MtlLoader::MtlLoader()
{
}

MtlLoader::~MtlLoader()
{
}

bool MtlLoader::LoadFromFile(std::string filename)
{
	LoadMaterialMap(filename);
	return false;
}

SkyVRMtl MtlLoader::LoadFromFile(std::string filename, std::string mtlName)
{
	LoadMaterialMap(filename);
	return mtlMap[mtlName];
}

bool MtlLoader::LoadMaterialMap(std::string filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		UE_LOG(LogTemp, Warning, TEXT("Error finding/opening mtl file."));
		return false;
	}

	std::string path = filename.substr(0, filename.find_last_of("/") + 1);
	std::string line;
	std::string buf;

	bool mtlOpen = false;
	bool mtlSaved = false;

	std::string mtlName;
	FVector Ka;
	FVector Kd;
	FVector Ks;
	float d;
	float illum;
	std::string diffuseMap;

	while (std::getline(file, line)) {
		const char *lch = line.c_str();
		char head[128];
		sscanf(lch, "%s", head);

		if (strcmp(head, "newmtl") == 0) {
			// New material coming, add previous mtl to map
			if (mtlOpen && !mtlSaved) {
				std::string diffusePath = path;
				SkyVRMtl newmtl(mtlName, diffusePath.append(diffuseMap));
				newmtl.ambient = Ka;
				newmtl.diffuse = Kd;
				newmtl.specular = Ks;
				newmtl.d = d;
				newmtl.illumination = illum;
				mtlMap[mtlName] = newmtl;
				mtlSaved = true;
			}
			// Read name
			std::stringstream ss(line);
			ss >> buf;
			ss >> mtlName;
			mtlOpen = true;
			mtlSaved = false;
		}
		else if (strcmp(head, "Ka") == 0) {
			// Read ambient
			const char *ch = line.substr(3).c_str();
			sscanf_s(ch, "%f %f %f\n", &Ka.X, &Ka.Y, &Ka.Z);
		}
		else if (strcmp(head, "Kd") == 0) {
			// Read diffuse
			const char *ch = line.substr(3).c_str();
			sscanf_s(ch, "%f %f %f\n", &Kd.X, &Kd.Y, &Kd.Z);
		}
		else if (strcmp(head, "Ks") == 0) {
			// Read specular
			const char *ch = line.substr(3).c_str();
			sscanf_s(ch, "%f %f %f\n", &Ks.X, &Ks.Y, &Ks.Z);
		}
		else if (strcmp(head, "d") == 0) {
			// Read d
			const char *ch = line.substr(2).c_str();
			sscanf_s(ch, "%f", &d);
		}
		else if (strcmp(head, "illum") == 0) {
			// Read illumination
			const char *ch = line.substr(6).c_str();
			sscanf_s(ch, "%f", &illum);
		}
		else if (strcmp(head, "map_Kd") == 0) {
			// Read diffuse map
			std::stringstream ss(line);
			ss >> buf;
			ss >> diffuseMap;
			//            std::cout << "diffuse map " << diffuseMap << std::endl;
		}
	}
	file.close();

	if (mtlOpen && !mtlSaved) {
		std::string diffusePath = path;
		SkyVRMtl newmtl(mtlName, diffusePath.append(diffuseMap));
		newmtl.ambient = Ka;
		newmtl.diffuse = Kd;
		newmtl.specular = Ks;
		newmtl.d = d;
		newmtl.illumination = illum;
		mtlMap[mtlName] = newmtl;
		mtlSaved = true;
	}

	fileRead = true;
	return true;
}