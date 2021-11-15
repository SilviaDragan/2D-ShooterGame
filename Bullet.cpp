#include "lab_m1/tema1/Bullet.h"


using namespace m1;


Bullet::Bullet() {
}


Bullet::~Bullet() {
}

Mesh* Bullet::generateBulletMesh() {
	return object2D::CreateSquare("projectile", glm::vec3(0, 0, 0), 20, glm::vec3(0, 0, 0), true);
}
