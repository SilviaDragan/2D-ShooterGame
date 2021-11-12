#pragma once
#include "components/simple_scene.h"
#include "lab_m1/tema1/Map.h"
#include "lab_m1/tema1/Enemy.h"
#include "lab_m1/tema1/Bullet.h"
#include "utils/math_utils.h"
#include <stdlib.h>
#include <vector>
#include <time.h>    
#include <iostream>
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
        void Tema1::DrawMap(glm::mat3 visMatrix);
        void Tema1::DrawPlayer(glm::mat3 visMatrix, float deltaTimeSeconds);
        void Tema1::DrawEnemy(Enemy* enemy, glm::mat3 visMatrix, float deltaTimeSeconds);

    protected:
        // variables here
        glm::ivec2 resolution;
        glm::mat3 playerBodyModelMatrix, playerPartsModelMatrix;
        glm::mat3 visMatrix;
        glm::mat3 mapModel, projectileMatrix;
        float playerSquareSide, playerSmallPartsSquareSide;
        float transPlayerX, transPlayerY, transSmallPartsX, transSmallPartsY;
        float mouseAngle;

        Map* map = new Map();
        float mapLength, mapScaleFactor, obstacleLength;
        float mapCorner;
        float mapScaleX, mapScaleY;

        //enemy just for rendering meshes
        Enemy* enemy;

        vector<Enemy*> enemies;
        glm::mat3 enemyBodyModelMatrix;
        glm::mat3 enemyArmsModelMatrix;
        float enemySpeed; // cred ca poate fi sters
        float enemyPozX, enemyPozY;
        float enemyBodySquareSide;


        clock_t currentTime;
        clock_t lastTime;
        float timeCount;
        

        vector<Bullet*> bullets;
        /*float projectileLength, transProjectileX, transProjectileY, projectilePozitionX, projectilePozitionY;
        bool spawnProjectile;
        float projectileAngle, projectileSpeed = 100;
        float projectileIntialX, projectileIntialY;*/
        bool spawnNewProjectile;



        ViewportSpace viewSpace;
        LogicSpace logicSpace;
    };
}   // namespace m1