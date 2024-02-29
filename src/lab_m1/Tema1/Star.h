#pragma once

#include "components/simple_scene.h"

namespace m1
{
	class Star : public gfxc::SimpleScene
	{
	public:
		Mesh* mesh;
		float x, y, scaleX, scaleY;
		glm::mat3 modelMatrix;
		float angular;
		int line;
		int color;
		Star(Mesh* newMesh, float newX, float newY, float newScaleX, float newScaleY, glm::mat3 newModelMatrix, int line, int color);
		~Star();
	};
}