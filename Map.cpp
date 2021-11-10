#include "lab_m1/tema1/Map.h"


using namespace std;
using namespace m1;

Map::Map() {
}


Map::~Map() {
}

Mesh* Map::generateMapMesh(float mapLength) {
	return object2D::CreateSquare("map", glm::vec3(0, 0, 0), mapLength, glm::vec3(0.25f, 0, 0.75f));
}

Mesh* Map::generateObstacleMesh(float obstacleLength) {
	return object2D::CreateSquare("obstacle", glm::vec3(0, 0, 0), obstacleLength, glm::vec3(0, 0.25f, 0.5f), true);
}
