#pragma once

#include "components/simple_scene.h"

namespace m1
{
	class Enemy : public gfxc::SimpleScene
	{
	public:
		Mesh* mesh;
		float x, y;
		glm::mat3 modelMatrix;
		int line;
		int color;
		int lifePoints = 3;

		float scaleX = 1.f;
		float scaleY = 1.f;

		bool toDelete = false;
		Enemy(Mesh* newMesh, float newX, float newY, glm::mat3 newModelMatrix, int line, int newColor);
		~Enemy();
	};
}