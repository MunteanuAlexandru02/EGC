#include "lab_m1/Tema1/Plant.h"

#include <iostream>

using namespace std;
using namespace m1;

Plant::Plant(Mesh* newMesh, int newX, int newY, int newProjIndex, int newColumnIndex, int newRowIndex, glm::mat3 newModelMatrix, int newColor)
{
	mesh = newMesh;
	x = newX;
	y = newY;
	modelMatrix = newModelMatrix;
	projectileIndex = newProjIndex;
	columnIndex = newColumnIndex;
	rowIndex = newRowIndex;
	color = newColor;
}

Plant::~Plant()
{
}