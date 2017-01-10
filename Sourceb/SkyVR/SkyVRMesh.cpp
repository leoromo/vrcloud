// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyVR.h"
#include "SkyVRMesh.h"
#include "MtlLoader.h"
#include <fstream>
#include <sstream>
#include <Regex.h>

SkyVRMesh::SkyVRMesh()
{
}

SkyVRMesh::~SkyVRMesh()
{
}

bool SkyVRMesh::LoadFromFile(const std::string filename)
{
	// Open file
	std::ifstream objFile(filename);

	if (!objFile.is_open()) {
		UE_LOG(LogTemp, Warning, TEXT("Error finding/opening obj file."));
		return false;
	}

	// Read and fill containers
	std::string line;
	while (std::getline(objFile, line)) {
		const char *lch = line.c_str();
		char head[128];
		sscanf(lch, "%s", head);

		if (strcmp(head, "g") == 0) {
			// Group
		}
		else if (strcmp(head, "mtllib") == 0) {
			// Mtl file
			std::stringstream ss(line);
			std::string buf;
			ss >> buf;
			ss >> mtlFile;
		}
		else if (strcmp(head, "usemtl") == 0) {
			// Mtl name
			std::stringstream ss(line);
			std::string buf;
			ss >> buf;
			ss >> mtlName;
		}
		else if (strcmp(head, "v") == 0) {
			// Vertex
			FVector v;
			std::string coords = line.substr(2);
			FRegexPattern pattern(TEXT("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
			FRegexMatcher matcher(pattern, FString(coords.c_str()));
			matcher.FindNext();
			v.X = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			matcher.FindNext();
			v.Y = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			matcher.FindNext();
			v.Z = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			vertices.Add(v);
		}
		else if (strcmp(head, "vt") == 0) {
			// UVs
			FVector2D uv;
			std::string coords = line.substr(3);
			FRegexPattern pattern(TEXT("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
			FRegexMatcher matcher(pattern, FString(coords.c_str()));
			matcher.FindNext();
			uv.X = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			matcher.FindNext();
			uv.Y = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			// Invert v coords
			uv.Y = 1 - uv.Y;
			uvs.Add(uv);
		}
		else if (strcmp(head, "vn") == 0) {
			// Normal
			FVector normal;
			std::string coords = line.substr(3);
			FRegexPattern pattern(TEXT("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
			FRegexMatcher matcher(pattern, FString(coords.c_str()));
			matcher.FindNext();
			normal.X = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			matcher.FindNext();
			normal.Y = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			matcher.FindNext();
			normal.Z = std::stof(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning()));
			normals.Add(normal);
		}
		else if (strcmp(head, "f") == 0) {
			// Face
			
			std::string coords = line.substr(2);
			FRegexPattern pattern(TEXT("[0-9]*/[/0-9]*"));
			FRegexMatcher matcher(pattern, FString(coords.c_str()));
			matcher.FindNext();
			indices.Add(std::stoi(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning())) - 1);
			matcher.FindNext();
			indices.Add(std::stoi(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning())) - 1);
			matcher.FindNext();
			indices.Add(std::stoi(coords.substr(matcher.GetMatchBeginning(), matcher.GetMatchEnding() - matcher.GetMatchBeginning())) - 1);
		}
	}

	// Get material full path
	std::string path = filename.substr(0, filename.find_last_of("\\") + 1);
	std::string mtlFileName = mtlFile.substr(mtlFile.find_first_of("\\") + 1);
	mtlFile = path + mtlFileName;

	// Read material
	mtl = MtlLoader().LoadFromFile(mtlFile, mtlName);

	// Read texture

	return true;
}


