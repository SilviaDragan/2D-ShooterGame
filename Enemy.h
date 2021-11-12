#pragma once
#include "glm/vec3.hpp"
#include "../src/core/gpu/mesh.h"
#include "lab_m1/lab3/object2D.h"

namespace m1 {
	class Enemy {
	public:
		Enemy();
		~Enemy();
		Mesh* generateEnemyBodyMesh();
		Mesh* generateEnemyArmsMesh();
		float pozX, pozY;
		float enemyBodySquareSide = 80;
		float enemyArmsSquareSide = 40;
	};

}