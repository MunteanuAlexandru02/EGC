#pragma once

#include "components/simple_scene.h"

namespace m1
{
	class Projectile : public gfxc::SimpleScene
	{
	public:
		glm::vec3 projectilePosition;
		glm::vec3 projectileScale;
		float turretRotation{ 0.0 };
		float scale = { 0.08 };

		Projectile(glm::vec3 position, glm::vec3 scaling, float turret);
		~Projectile();
	};
}