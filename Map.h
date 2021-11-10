#pragma once
#include "glm/vec3.hpp"
#include "../src/core/gpu/mesh.h"
#include "lab_m1/lab3/object2D.h"

namespace m1 {
	class Map {
		public:
			Map();
			~Map();
			Mesh* generateMapMesh(float mapLength);
			Mesh* generateObstacleMesh(float obstacleLength);
	};

}