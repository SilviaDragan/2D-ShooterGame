#include "lab_m1/tema1/Enemy.h"


using namespace m1;


Enemy::Enemy() {
}


Enemy::~Enemy() {
}

Mesh* Enemy::generateEnemyBodyMesh() {
	return object2D::CreateSquare("enemyBody", glm::vec3(0, 0, 0), enemyBodySquareSide, glm::vec3(0.2f, 0, 0.2f), true);
}

Mesh* Enemy::generateEnemyArmsMesh() {
	return object2D::CreateSquare("enemyArm", glm::vec3(0, 0, 0), enemyArmsSquareSide, glm::vec3(0.1f, 0, 0.1f), true);
}