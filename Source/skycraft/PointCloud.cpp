// Fill out your copyright notice in the Description page of Project Settings.

#include "skycraft.h"
#include "PointCloud.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int PointCloud::ReadFromFile(const std::string filename)
{
	// Open filestream
	ifstream file(filename, ifstream::in);
	if (!file.is_open())
	{
		cerr << "Error opening point cloud file" << endl;
		return -1;
	}
	string line;
	vector<string> lineData;
	vector<Point> pointData;
	char separator = ',';

	//Prepare variables for min, max values
	Point min, max, centroid;
	min.x = min.y = min.z = numeric_limits<float>::max();
	max.x = max.y = max.z = -numeric_limits<float>::max();
	
	while (getline(file, line))
	{
		lineData.clear();
		stringstream ss(line);
		string value;
		while (getline(ss, value, separator))
		{
			lineData.push_back(value);
		}
		//lineData must have 6 values at this point
		if (lineData.size() != 6)
		{
			//ignore point
			continue;
		}
		Point p;
		//try
		//{
			p.x = stof(lineData[0]);
			p.y = stof(lineData[1]);
			p.z = stof(lineData[2]);
			p.r = stoi(lineData[3]);
			p.g = stoi(lineData[4]);
			p.b = stoi(lineData[5]);
		//}
		//catch (const std::invalid_argument&)
		//{
		//	//Ignore point
		//	continue;
		//}
		// Update min & max values
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
		//Update centroid
		centroid.x += p.x;
		centroid.y += p.y;
		centroid.z += p.z;
		pointData.push_back(p);
	}
	centroid.x /= pointData.size();
	centroid.y /= pointData.size();
	centroid.z /= pointData.size();

	this->points = pointData;
	this->min = min;
	this->max = max;
	this->centroid = centroid;
	return 1;
}

int PointCloud::WriteToFile(const std::string filename)
{
	//Open filestream
	ofstream file(filename, ofstream::out);
	if (!file.is_open())
	{
		cerr << "Error opening output file" << std::endl;
		return -1;
	}
	for (unsigned int i = 0; i < this->points.size(); ++i)
	{
		file << this->points[i] << std::endl;
	}
	return 1;
}

void PointCloud::DeMean()
{
	std::vector<Point> centered;
	Point newCentroid;
	if (centroid.x != 0 || centroid.y != 0 || centroid.z != 0)
	{
		for (int i = 0; i < points.size(); ++i)
		{
			newCentroid = newCentroid + points[i];
		}
		newCentroid = newCentroid / points.size();
		for (int i = 0; i < points.size(); ++i)
		{
			centered.push_back(points[i] - newCentroid);
		}
	}
	centroid = newCentroid;
	points = centered;
	std::cout << centroid << std::endl;
}

int PointCloud::Voxelize(float voxelSize)
{
	//Allocate voxels
	int sx = static_cast<int>((max.x - min.x) * (1.0 / voxelSize)) + 1;
	int sy = static_cast<int>((max.y - min.y) * (1.0 / voxelSize)) + 1;
	int sz = static_cast<int>((max.z - min.z) * (1.0 / voxelSize)) + 1;

	//Prepare voxel structure
	std::vector<std::vector<std::vector<std::vector<int> > > > voxels;
	//Allocate memory for x 
	voxels.resize(sz);
	for (int z = 0; z<sz; ++z)
	{
		voxels[z].resize(sy);
		for (int y = 0; y<sy; ++y)
		{
			voxels[z][y].resize(sx);
		}
	}
	// Push points into voxels
	for (unsigned int i = 0; i < this->points.size(); ++i)
	{
		int x = static_cast<int>((points[i].x - min.x) / voxelSize);
		int y = static_cast<int>((points[i].y - min.y) / voxelSize);
		int z = static_cast<int>((points[i].z - min.z) / voxelSize);
		voxels[z][y][x].push_back(i);
	}
	// Iterate through all voxels and create a new point cloud
	std::vector<Point> newCloud;
	for (unsigned int z = 0; z < voxels.size(); ++z)
	{
		for (unsigned int y = 0; y < voxels[z].size(); ++y)
		{
			for (unsigned int x = 0; x < voxels[z][y].size(); ++x)
			{
				if (voxels[z][y][x].size() > 0)
				{
					Point voxel;
					voxel.x = (voxelSize * x) + min.x + voxelSize / 2.0;
					voxel.y = (voxelSize * y) + min.y + voxelSize / 2.0;
					voxel.z = (voxelSize * z) + min.z + voxelSize / 2.0;
					voxel.CopyColor(GetVoxelColor(voxels[z][y][x]));
					newCloud.push_back(voxel);
				}
			}
		}
	}

	std::cout << "Point cloud size = " << points.size() << std::endl;
	std::cout << "Voxel cloud size = " << newCloud.size() << std::endl;

	this->points = newCloud;

	return 0;
}

Point PointCloud::GetVoxelColor(std::vector<int> population)
{
	int r, g, b;
	r = g = b = 0;
	for (unsigned int i = 0; i < population.size(); ++i)
	{
		r += points[population[i]].r;
		g += points[population[i]].g;
		b += points[population[i]].b;
	}
	r /= population.size();
	g /= population.size();
	b /= population.size();
	Point p;
	p.r = r;
	p.g = g;
	p.b = b;
	return p;
}
