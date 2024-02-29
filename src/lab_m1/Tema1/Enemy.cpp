#include "lab_m1/Tema1/Enemy.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Enemy::Enemy(Mesh* newMesh, float newX, float newY, glm::mat3 newModelMatrix, int newLine, int newColor)
{
	mesh = newMesh;
	x = newX;
	y = newY;
	modelMatrix = newModelMatrix;
	color = newColor;
	line = newLine;
}

Enemy::~Enemy()
{
}

