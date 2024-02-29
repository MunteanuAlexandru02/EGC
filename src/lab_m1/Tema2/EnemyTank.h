#pragma once

#include "components/simple_scene.h"

enum states
{
	FORWARD,
	BACKWARD,
	ROTATE_LEFT,
	ROTATE_RIGHT
};

namespace m1
{
	class EnemyTank : public gfxc::SimpleScene
	{
	public:
		glm::vec3 enemyPosition = {0.0, 0.0, 0.0};
		glm::vec3 angularStep{ 0.0, 0.0, 0.0 };
		float turretRotation{ 0.0 };
		float cannonRotation{ 0.0 };

		float lastShot = 0; // the last time the tank shot

		int health = 3;

		states state;
		
		EnemyTank(glm::vec3 position);
		~EnemyTank();
	};
}