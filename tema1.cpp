#include "lab_m1/tema1/tema1.h";

using namespace std;
using namespace m1;

#define MAX_ENEMIES 10
#define MAX_BULLETS 5


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);


    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = resolution.x;   // logic width
    logicSpace.height = resolution.y;  // logic height

    glm::vec3 corner = glm::vec3(1, 0, 0);
    playerSquareSide = 100, playerSmallPartsSquareSide = 20;
    transPlayerX = (resolution.x - playerSquareSide) / 2;
    transPlayerY = (resolution.y - playerSquareSide) / 2;

    mouseAngle = 0;

    mapLength = 1000;  
    mapCorner = -150;
    obstacleLength = 200;
    mapScaleX = 2; mapScaleY = 1;

    currentTime = clock();
    lastTime = currentTime;
    timeCount = 0;

    Bullet* bullet = new Bullet();
    bullet->projectileLength = 20;
    spawnProjectile = false;
    bullets.resize(MAX_BULLETS);
    /*transProjectileX = projectileLength;
    transProjectileY = -projectileLength;*/

    /*projectilePozitionX = (resolution.x - playerSquareSide) / 2 + projectileLength;
    projectilePozitionY = (resolution.y - playerSquareSide) / 2 -projectileLength;

    projectileIntialX = (resolution.x - playerSquareSide + projectileLength) / 2;
    projectileIntialY = (resolution.y - playerSquareSide + projectileLength) / 2;*/

    Mesh* playerBody = object2D::CreateSquare("playerBody", glm::vec3(0, 0, 0), playerSquareSide, glm::vec3(0.1f, 0.4f, 0.3f), true);
    Mesh* playerSmallPart = object2D::CreateSquare("playerSmallPart", glm::vec3(0, 0, 0), playerSmallPartsSquareSide, glm::vec3(0, 0.6f, 0.3f), true);

    // enemy just for rendering meshez
    enemy = new Enemy();
    // initialize enemies list
    enemies.resize(MAX_ENEMIES);

  

    AddMeshToList(playerBody);
    AddMeshToList(playerSmallPart);
    AddMeshToList(map->generateMapMesh(mapLength));
    AddMeshToList(map->generateObstacleMesh(obstacleLength));
    AddMeshToList(bullet->generateBulletMesh());
    AddMeshToList(enemy->generateEnemyBodyMesh());
    AddMeshToList(enemy->generateEnemyArmsMesh());
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds) 
{

    // pentru fereastra
    resolution = window->GetResolution();

    //// Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    //// Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix, deltaTimeSeconds);

}

void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    DrawMap(visMatrix);
    DrawPlayer(visMatrix, deltaTimeSeconds);

    // la fiecare 15 secunde spawnez un inamic nou pe harta
    currentTime = clock();
    timeCount += (int) (currentTime - lastTime);
    lastTime = currentTime;
    
    if (timeCount >= 10 * CLOCKS_PER_SEC) {
        //cout << "yes" << endl;
        // spawn new enemy at random place in map
        int min = mapCorner + enemyBodySquareSide;
        int maxX = mapCorner + mapLength * mapScaleX - enemyBodySquareSide;
        int maxY = mapCorner + mapLength * mapScaleY - enemyBodySquareSide;


        Enemy* newEnemy = new Enemy();
        newEnemy->pozX = rand() % maxX  + min;
        newEnemy->pozY = rand() % maxY + min;
        newEnemy->defeated = false;
        // give enemy random speed
        newEnemy->speed = rand()% 200 + 50;
        //cout << newEnemy->speed << endl;

        enemies.push_back(newEnemy);
        DrawEnemy(newEnemy, visMatrix, deltaTimeSeconds);
        
        // reset  enemy spawn timer
        timeCount = 0;

    }

    // draw and move alive enemies 
    //for (int i = 0; i < enemies.size(); i++ ) {
    //    Enemy* enemy = enemies[i];
    //    
    //        // change enemy pozition before drawing
     

        
    //}
    

    
    if (spawnNewProjectile) {
        Bullet* newBullet = new Bullet();
        newBullet->angle = mouseAngle;
        newBullet->speed = 100;
        newBullet->initialX = resolution.x / 2;
        newBullet->initialY = resolution.y / 2;
        newBullet->positionX = resolution.x / 2;
        newBullet->positionY = resolution.y / 2;
        newBullet->transProjectileX = deltaTimeSeconds * sin(newBullet->angle);
        newBullet->transProjectileY = deltaTimeSeconds * (-cos(newBullet->angle));

        bullets.push_back(newBullet);
        spawnNewProjectile = false;
    }

    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        if (b->positionX < mapLength && b->positionY < mapLength) {
            b->positionX += b->transProjectileX;
            b->positionY += b->transProjectileY;
        }
        else {
            bullets.erase(bullets.begin() + i);
        }
        projectileMatrix =
            visMatrix *
            transform2D::Translate(b->positionX, b->positionY) *
            transform2D::Rotate(b->angle);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectileMatrix);
    }
        

        //transProjectileX = deltaTimeSeconds * sin(mouseAngle);
        //transProjectileY = deltaTimeSeconds * (-cos(mouseAngle));

        //if (projectilePozitionX < mapLength && projectilePozitionY < mapLength) {
        //    projectilePozitionX += transProjectileX;
        //    projectilePozitionY += transProjectileY;
        //}
        //else {
        //    spawnProjectile = false;
        //    // reset bullet pozition 
        //    projectilePozitionX = projectileIntialX;
        //    projectilePozitionY = projectileIntialY;
        //}


        /*projectileMatrix =
            visMatrix *
            transform2D::Translate(projectilePozitionX, projectilePozitionY) *
            transform2D::Rotate(projectileAngle);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectileMatrix);*/

    }
    
}

void Tema1::DrawMap(glm::mat3 visMatrix) {
    mapModel = glm::mat3(1);
    mapModel *=
        visMatrix *
        transform2D::Translate(mapCorner, mapCorner) *
        transform2D::Scale(mapScaleX, mapScaleY);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], mapModel);


    mapModel = glm::mat3(1);
    mapModel *=
        visMatrix *
        transform2D::Translate(100, -50) *
        transform2D::Scale(1.75f, 0.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], mapModel);

    mapModel = glm::mat3(1);
    mapModel *=
        visMatrix *
        transform2D::Translate(800, 200) *
        transform2D::Scale(0.5f, 1.75f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], mapModel);

    mapModel = glm::mat3(1);
    mapModel *=
        visMatrix *
        transform2D::Translate(100, 500) *
        transform2D::Scale(2.5f, 0.25);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], mapModel);
}

void Tema1::DrawPlayer(glm::mat3 visMatrix, float deltaTimeSeconds) {

    playerBodyModelMatrix = glm::mat3(1);
    playerPartsModelMatrix = glm::mat3(1);

    playerBodyModelMatrix =
        transform2D::Translate((resolution.x - playerSquareSide) / 2, (resolution.y - playerSquareSide) / 2) *
        transform2D::Translate(playerSquareSide / 2, playerSquareSide / 2) *
        transform2D::Rotate(mouseAngle) *
        transform2D::Translate(-playerSquareSide / 2, -playerSquareSide / 2);

    RenderMesh2D(meshes["playerBody"], shaders["VertexColor"], playerBodyModelMatrix);

    playerPartsModelMatrix = playerBodyModelMatrix;
    playerPartsModelMatrix *= transform2D::Translate(playerSquareSide, 0);

    RenderMesh2D(meshes["playerSmallPart"], shaders["VertexColor"], playerPartsModelMatrix);

    playerPartsModelMatrix = playerBodyModelMatrix;
    playerPartsModelMatrix *= transform2D::Translate(playerSquareSide, playerSquareSide - playerSmallPartsSquareSide);

    RenderMesh2D(meshes["playerSmallPart"], shaders["VertexColor"], playerPartsModelMatrix);
   
}

void Tema1::DrawEnemy(Enemy* enemy, glm::mat3 visMatrix, float deltaTimeSeconds) { 
    
    enemyArmsModelMatrix = glm::mat3(1);
    //enemyArmsModelMatrix = visMatrix * transform2D::Translate(-80, -90);
    enemyArmsModelMatrix = visMatrix * transform2D::Translate(enemy->pozX + 60, enemy->pozY + 50);
    RenderMesh2D(meshes["enemyArm"], shaders["VertexColor"], enemyArmsModelMatrix);

    enemyArmsModelMatrix = glm::mat3(1);
    //enemyArmsModelMatrix = visMatrix * transform2D::Translate(-80, -150);
    enemyArmsModelMatrix = visMatrix * transform2D::Translate(enemy->pozX + 60, enemy->pozY - 10);
    RenderMesh2D(meshes["enemyArm"], shaders["VertexColor"], enemyArmsModelMatrix);


    enemyBodyModelMatrix = glm::mat3(1);
    enemyBodyModelMatrix = visMatrix * transform2D::Translate(enemy->pozX, enemy->pozY);
    //enemyBodyModelMatrix = visMatrix * transform2D::Translate(-140, -140);
    RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], enemyBodyModelMatrix);
}

void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods){
    // Move the logic window with W, A, S, D (up, left, down, right)
    if (window->KeyHold(GLFW_KEY_W)) {
        logicSpace.y += 100 * deltaTime;
        transPlayerY += 100 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        logicSpace.y -= 100 * deltaTime;
        transPlayerY -= 100 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_A)) {
        logicSpace.x -= 100 * deltaTime;
        transPlayerX -= 100 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_D)) {
        logicSpace.x += 100 * deltaTime;
        transPlayerX += 100 * deltaTime;

    }

    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    resolution = window->GetResolution();
    mouseAngle = atan2(mouseX - resolution.x/2, mouseY - resolution.y/2);
    mouseAngle -= RADIANS(90);

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        spawnNewProjectile = true;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
    // update coordinates according to window size
    resolution = window->GetResolution();


}
