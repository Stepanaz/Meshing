#include <iostream>
#include "Decimate.h"
#include "Gyroid.h"
#include "Sphere.h"
#include <fstream>

using namespace std;

int main(void)
{
	mesh setka;
	Gyroid surface;
	float xMin = -5;
	float yMin = -5;
	float zMin = -5;
	float xMax = 5;
	float yMax = 5;
	float zMax = 5;
	float isolevel = -1;
	size_t resolution = 20;

	decimate(setka, surface, xMin, xMax, yMin, yMax, zMin, zMax, isolevel, resolution);

	std::ofstream myfile;

	myfile.open("mesh.txt");
	Vertex tmp;
	list <Vertex>::iterator Iter;
	for (Iter = setka.Vertices.begin(); Iter != setka.Vertices.end(); Iter++)
	{
		tmp = *Iter;
		myfile << tmp.x << " " << tmp.y << " " << tmp.z << " \n";
	}


	myfile.close();

	return 0;
}