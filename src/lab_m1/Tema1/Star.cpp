#include "lab_m1/Tema1/Star.h"

#include <iostream>

using namespace std;
using namespace m1;

Star::Star(Mesh* newMesh, float newX, float newY, float newScaleX, float newScaleY, glm::mat3 newModelMatrix, int newLine, int newColor)
{
	mesh = newMesh;
	x = newX;
	y = newY;
	scaleX = newScaleX;
	scaleY = newScaleY;
	line = newLine;
	angular = 0;
	color = newColor;
	modelMatrix = newModelMatrix;
}
Star::~Star()
{
}