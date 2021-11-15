#pragma once
#include "glm/vec3.hpp"
#include "../src/core/gpu/mesh.h"
#include "lab_m1/lab3/object2D.h"

namespace m1 {
	class Bullet {
	public:
		Bullet();
		~Bullet();
		Mesh* generateBulletMesh();

		float projectileLength, transProjectileX, transProjectileY, positionX, positionY;
		float angle;
		float speed;
		float initialX, initialY;
		float distance;

	};

}