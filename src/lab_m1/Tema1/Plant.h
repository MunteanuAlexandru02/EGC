#pragma once

#include "components/simple_scene.h"

namespace m1
{
	class Plant : public gfxc::SimpleScene
	{
	public:
		Mesh* mesh;
		int x, y, projectileIndex; // coordinates in columns and rows vectors
		glm::mat3 modelMatrix;

		int columnIndex, rowIndex;
		int color;
		float scaleX = 1.f;
		float scaleY = 1.f;

		bool toDelete = false;

		float angular;

		Plant(Mesh* newMesh, int newX, int newY, int newProjIndex, int newColumnIndex, int newRowIndex, glm::mat3 newModelMatrix, int newColor);
		~Plant();
	};
}