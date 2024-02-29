#include "lab_m1/Tema2/Projectile.h"
#include <iostream>

using namespace m1;
using namespace std;

Projectile::Projectile(glm::vec3 position, glm::vec3 scaling, float turret) {
	projectilePosition = position;
	projectileScale = scaling;
	turretRotation = turret;
}

Projectile::~Projectile() {};
