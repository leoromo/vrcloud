// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include <iostream>

class SKYCRAFT_API Point
{
public:
	Point()
	{
		x = y = z = 0;
		r = g = b = 0;
	};
	~Point() {};

	float x;
	float y;
	float z;
	int r;
	int g;
	int b;

	friend std::ostream& operator<<(std::ostream& os, const Point &p)
	{
		os << p.x << "," << p.y << "," << p.z << "," << p.r << "," << p.g << "," << p.b;
		return os;
	}

	void CopyColor(const Point p)
	{
		this->r = p.r;
		this->g = p.g;
		this->b = p.b;
	}

	Point operator+(const Point& p)
	{
		Point np;
		np.x = this->x + p.x;
		np.y = this->y + p.y;
		np.z = this->z + p.z;
		np.r = this->r;
		np.g = this->g;
		np.b = this->b;
		return np;
	}

	Point operator-(const Point& p)
	{
		Point np;
		np.x = this->x - p.x;
		np.y = this->y - p.y;
		np.z = this->z - p.z;
		np.r = this->r;
		np.g = this->g;
		np.b = this->b;
		return np;
	}

	Point operator/(const float& f)
	{
		Point np;
		np.x = this->x / f;
		np.y = this->y / f;
		np.z = this->z / f;
		np.r = this->r;
		np.g = this->g;
		np.b = this->b;
		return np;
	}
};

class SKYCRAFT_API PointCloud
{
public:
	PointCloud() {};
	~PointCloud() {};
	std::vector<Point> points;

	int ReadFromFile(const std::string filename);
	int WriteToFile(const std::string filename);
	int Voxelize(float voxelSize = 0.25);

	void DeMean();

	Point min;
	Point max;
	Point centroid;
private:
	Point GetVoxelColor(std::vector<int> population);
};
