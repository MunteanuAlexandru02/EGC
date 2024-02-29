#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    float length = 40.f;
    float width = 380.f;

    cx = (corner.x + squareSide) / 2.0;
    cy = (corner.y + squareSide) / 2.0;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* grass = object2D::CreateSquare("grass", corner, squareSide, glm::vec3(0.1176f, 0.84f, 0.2741f), true);
    AddMeshToList(grass);

    Mesh* hearth = object2D::CreateSquare("hearth", corner, squareSide, glm::vec3(0.81f, 0.0324f, 0.149f), true);
    AddMeshToList(hearth);

    Mesh* barrier = object2D::CreateRectangle("barrier", corner, length, width, glm::vec3(0.94, 0.0564, 0.1448), true);
    AddMeshToList(barrier);

    Mesh* star = object2D::CreateStar("pointStar", 35.f, 15.f,glm::vec3(1.f, 0.9713, 0.14));
    AddMeshToList(star);

    /* Build projectile meshes */
    Mesh* orangeStar = object2D::CreateStar("orangeStar", 25.f, 15.f, glm::vec3(0.98, 0.3621, 0.0294), true);
    AddMeshToList(orangeStar);

    Mesh* blueStar = object2D::CreateStar("blueStar", 25.f, 15.f, glm::vec3(0.0294, 0.3621, 0.98), true);
    AddMeshToList(blueStar);

    Mesh* purpleStar = object2D::CreateStar("purpleStar", 25.f, 15.f, glm::vec3(0.6314, 0.0294, 0.98), true);
    AddMeshToList(purpleStar);

    Mesh* yellowStar = object2D::CreateStar("yellowStar", 25.f, 15.f, glm::vec3(0.9351, 0.95, 0.057), true);
    AddMeshToList(yellowStar);

    Mesh* costStar = object2D::CreateStar("costStar", 15.f, 5.f, glm::vec3(0.41, 0.41, 0.41));
    AddMeshToList(costStar);
    
    /* Build plant meshes */
    Mesh* orange_plant = object2D::CreatePlant("orange_plant", glm::vec3(0.98, 0.3621, 0.0294), true);
    AddMeshToList(orange_plant);

    Mesh* blue_plant = object2D::CreatePlant("blue_plant", glm::vec3(0.0294, 0.3621, 0.98), true);
    AddMeshToList(blue_plant);

    Mesh* purple_plant = object2D::CreatePlant("purple_plant", glm::vec3(0.6314, 0.0294, 0.98), true);
    AddMeshToList(purple_plant);

    Mesh* yellow_plant = object2D::CreatePlant("yellow_plant", glm::vec3(0.9351, 0.95, 0.057), true);
    AddMeshToList(yellow_plant);

    /* Build plant contour */
    Mesh* contour = object2D::CreateSquare("contour", corner, squareSide, glm::vec3(1.f, 1.f, 1.f));
    AddMeshToList(contour);

    /*Build enemy*/
    Mesh* orange_enemy = object2D::CreateEnemy("orange_enemy", 40.f, 30.f, glm::vec3(0.98, 0.3621, 0.0294), glm::vec3(0.0324, 0.81, 0.6804), true);
    AddMeshToList(orange_enemy);

    Mesh* blue_enemy = object2D::CreateEnemy("blue_enemy", 40.f, 30.f, glm::vec3(0.0294, 0.3621, 0.98), glm::vec3(0.0324, 0.81, 0.6804), true);
    AddMeshToList(blue_enemy);

    Mesh* yellow_enemy = object2D::CreateEnemy("yellow_enemy", 40.f, 30.f, glm::vec3(0.9351, 0.95, 0.057), glm::vec3(0.0324, 0.81, 0.6804), true);
    AddMeshToList(yellow_enemy);

    Mesh* purple_enemy = object2D::CreateEnemy("purple_enemy", 40.f, 30.f, glm::vec3(0.6314, 0.0294, 0.98), glm::vec3(0.0324, 0.81, 0.6804), true);
    AddMeshToList(purple_enemy);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    glm::ivec2 res = window->GetResolution();



    for (int i{ 0 }; i < 3; i++) {
        for (int j{ 0 }; j < 3; j++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(columns[i], rows[j]);
            RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);
        }
    }

    for (int i{ 0 }; i < 4; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gridPlantX[i], gridPlantY);
        RenderMesh2D(meshes["contour"], shaders["VertexColor"], modelMatrix);
    }

    int range;

    for (int i{ 0 }; i < 4; i++) {
        if (i == 0) range = 1;
        else if (i == 1 || i == 2) range = 2;
        else if (i == 3) range = 3;
        float copyGridPlant = gridPlantX[i];
        for (int j{ 0 }; j < range; j++) {
            if (j == 0) copyGridPlant += 15.f;
            else copyGridPlant += 35.f;
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(copyGridPlant, gridPlantY - 25.f);
            RenderMesh2D(meshes["costStar"], shaders["VertexColor"], modelMatrix);

        }
    }

    for (int i{ 0 }; i < 4; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gridPlantX[i] - cx / 2, gridPlantY);
        RenderMesh2D(meshes[plantNames[i]], shaders["VertexColor"], modelMatrix);
    }
    for (int i = 0; i < lifeCounter; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(hearthX[i], hearthY);
        RenderMesh2D(meshes["hearth"], shaders["VertexColor"], modelMatrix);
    }

    startCostStar = 720.f;
    for (int i = 0; i < points; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(startCostStar, hearthY - 25.f);
        RenderMesh2D(meshes["costStar"], shaders["VertexColor"], modelMatrix);
        startCostStar += 35.f;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(barrierX, barrierY);
    RenderMesh2D(meshes["barrier"], shaders["VertexColor"], modelMatrix);

    seconds = glfwGetTime();
    int x = seconds * 100;
    int randomTime = rand() % 20 + 1;


    if (x <= (int)seconds * 100 + 2 && x >= (int)seconds * 100) {
        if (x / 100 % randomTime == 0) {
            int enemy = rand() % 4;
            int yRow = rand() % 3;
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(enemyStartingPos, rows[yRow] + halfSquare);
            enemies.push_back(new Enemy(meshes[enemyNames[enemy]], enemyStartingPos, rows[yRow] + halfSquare, modelMatrix, yRow, enemy));
        }

        // Spawn stars
        if (x / 100 % 7 == 0) {
            for (int i = 0; i < 3; i++) {
                int starX = rand() % 1000 + 50;
                int starY = rand() % 600 + 50;
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(starX, starY);

                // Use enemy class for convenience
                stars.push_back(new Star(meshes["pointStar"], starX, starY, 1.f, 1.f, modelMatrix,0, 0));
            }
        }

        if (x / 100 % 3 == 0) {
            for (int i{ 0 }; i < plants.size(); i++) {
                
                //Spawn the stars and add them into a vector, them update the position, then render
                for (int j{ 0 }; j < enemies.size(); j++) {
                    // there is an enemy on a line with the plant and they are the same color
                    if (plants[i]->rowIndex == enemies[j]->line && plants[i]->color == enemies[j]->color) {
                        modelMatrix = glm::mat3(1);
                        projectile.push_back(new Star(meshes[projectileNames[plants[i]->projectileIndex]], 
                                                      columns[plants[i]->columnIndex] + 100, 
                                                      rows[plants[i]->rowIndex] + 50,
                                                      1, 1, modelMatrix, enemies[j]->line, enemies[j]->color));
                    }
                }
            }
        }
        // Shoot a star every X seconds from every plant
    }

    for (int i{ 0 }; i < projectile.size(); i++) {
        projectile[i]->modelMatrix = glm::mat3(1);
        projectile[i]->modelMatrix *= transform2D::Translate(projectile[i]->x, projectile[i]->y);
        projectile[i]->modelMatrix *= transform2D::Rotate(projectile[i]->angular);
        RenderMesh2D(projectile[i]->mesh, shaders["VertexColor"], projectile[i]->modelMatrix);

        for (int j{ 0 }; j < enemies.size(); j++) {
            if (projectile[i]->line == enemies[j]->line && projectile[i]->color == enemies[j]->color) {
                if (enemies[j]->x - projectile[i]->x < 65.f) {
                    // ok = false;
                    
                    cout << j << " " << enemies[j]->lifePoints;
                    projectile.erase(projectile.begin() + i, projectile.begin() + i + 1);
                    enemies[j]->lifePoints--;

                    if (enemies[j]->lifePoints == 0) {
                        //rescale the enemy
                        enemies[j]->toDelete = true;
                        //enemies.erase(enemies.begin() + j, enemies.begin() + j + 1);
                    }
                    break;
                }
            }
        }
    }

    if (enemies.size() > 0) {
        for (int i{ 0 }; i < enemies.size(); i++) {
            enemies[i]->modelMatrix = glm::mat3(1);
            enemies[i]->modelMatrix *= transform2D::Translate(enemies[i]->x, enemies[i]->y);
            enemies[i]->modelMatrix *= transform2D::Scale(enemies[i]->scaleX, enemies[i]->scaleY);
            RenderMesh2D(enemies[i]->mesh, shaders["VertexColor"], enemies[i]->modelMatrix);
            //if an enemy has passed the barrier, lose a life
            if (enemies[i]->x <= barrierX) {
                lifeCounter--;
                enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
            }
            //no more lives => end the game
            if (lifeCounter == 0)
                exit(1);
        }
    }

    if (plantIndexDrag >= 0) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(plantDragMouseX, plantDragMouseY);
        RenderMesh2D(meshes[plantNames[plantIndexDrag]], shaders["VertexColor"], modelMatrix);
    }

    

    for (int i{0}; i < plants.size(); i++) {
        plants[i]->modelMatrix *= transform2D::Scale(plants[i]->scaleX, plants[i]->scaleY);
        RenderMesh2D(plants[i]->mesh, shaders["VertexColor"], plants[i]->modelMatrix);

        for (int j{ 0 }; j < enemies.size(); j++) {
            if (enemies[j]->line == plants[i]->rowIndex)
                if (enemies[j]->x - plants[i]->x <= 100.f && enemies[j]->x > plants[i]->x) {
                    plants[i]->toDelete = true;
                    break;
                }
        }
    }
}

void Tema1::Update(float deltaTimeSeconds)
{
    if (enemies.size() > 0) 
        for (int i{ 0 }; i < enemies.size(); i++) {
            enemies[i]->x -= 50 * deltaTimeSeconds;
            if (enemies[i]->toDelete) {
                enemies[i]->scaleX -= 1 * deltaTimeSeconds;
                enemies[i]->scaleY -= 1 * deltaTimeSeconds;
                if (enemies[i]->scaleX <= 0.2)
                    enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
            }
        }

    // Delete a star when the scale is less than 0.991
    for (int i{ 0 }; i < stars.size(); i++) {
        if (stars[i]->scaleX <= 0.991)
            stars.erase(stars.begin() + i, stars.begin() + i + 1);
    }
    
    for (int i{ 0 }; i < stars.size(); i++) {
        stars[i]->modelMatrix *= transform2D::Scale(stars[i]->scaleX, stars[i]->scaleY);
        RenderMesh2D(stars[i]->mesh, shaders["VertexColor"], stars[i]->modelMatrix);
        stars[i]->scaleX -= 0.001 * deltaTimeSeconds;
        stars[i]->scaleY -= 0.001 * deltaTimeSeconds;
    }


    for (int i{ 0 }; i < plants.size(); i++) {
        if (plants[i]->toDelete == true) {
            plants[i]->scaleY -= 0.2 * deltaTimeSeconds;
            if (plants[i]->scaleY < 0.8) {
                isPlantPlaced[plants[i]->columnIndex][plants[i]->rowIndex] = false;
                plants.erase(plants.begin() + i, plants.begin() + i + 1);
            }
        }
    }

    cout << projectile.size() << endl;

    for (int i{ 0 }; i < projectile.size(); i++) {
        projectile[i]->x += 135 * deltaTimeSeconds;
        projectile[i]->angular -= 10 * deltaTimeSeconds;
        if (projectile[i]->x > 1400)
            projectile.erase(projectile.begin() + i, projectile.begin() + i + 1);
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if (plantIndexDrag >= 0) {
            plantDragMouseX += plantDragX;
            plantDragMouseY -= plantDragY;
            plantDragX = plantDragY = 0;
        }

    }
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
    plantDragX = deltaX;
    plantDragY = deltaY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    int yGame = 720 - mouseY;
    // For every press, check if there is a star there
    if (button == 1) {
        for (int i{ 0 }; i < stars.size(); i++) {
            // coordinates of the square
            int maxX = stars[i]->x + starMaxSize;
            int minX = stars[i]->x - starMaxSize;
            int maxY = stars[i]->y + starMaxSize;
            int minY = stars[i]->y - starMaxSize;
            if (minX <= mouseX && maxX >= mouseX) {
                if (minY <= yGame && maxY >= yGame) {
                    points++;
                    stars.erase(stars.begin() + i, stars.begin() + i + 1);
                }
            }
        }

        for (int i{ 0 }; i < 4; i++) {
            int minX = gridPlantX[i];
            int maxX = gridPlantX[i] + sideSquare;
            int minY = gridPlantY;
            int maxY = gridPlantY + sideSquare;
            // check if mouse click was inside the box
            if ((i == 0 && points >= 1) || ((i == 1 || i == 2) && points >= 2) || (i == 3 && points >= 3)) {
                if (minX <= mouseX && maxX >= mouseX) {
                    if (minY <= yGame && maxY >= yGame) {
                        plantIndexDrag = i;
                        plantDragMouseX = mouseX - 50;
                        plantDragMouseY = yGame - 50;
                        break;
                    }
                }
            }
        }
    }
    else {

        for (int i{ 0 }; i < plants.size(); i++) {
            int minX = columns[plants[i]->columnIndex];
            int maxX = columns[plants[i]->columnIndex] + sideSquare;
            int maxY = rows[plants[i]->rowIndex] + sideSquare;
            int minY = rows[plants[i]->rowIndex];

            if (minX <= mouseX && maxX >= mouseX) {
                if (minY <= yGame && maxY >= yGame) {
                    //start the scaling
                    plants[i]->toDelete = true;
                }
            }
        }
    }
    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    
    int yGame = 720 - mouseY;
    // Need to check if the plant was placed on grass
    if (plantIndexDrag >= 0) {
        for (int i{ 0 }; i < 3; i++) {
            for (int j{ 0 }; j < 3; j++) {
                int maxX = columns[i] + sideSquare;
                int minX = columns[i];
                int maxY = rows[j] + sideSquare;
                int minY = rows[j];

                if (minX <= mouseX && maxX >= mouseX) {
                    if (minY <= yGame && maxY >= yGame) {
                        //cout << "Am pus planta la " << i << " " << j << endl;
                        plantOnGrassX = i;
                        plantOnGrassY = j;
                        if (!isPlantPlaced[i][j]) {
                            if (plantIndexDrag == 0) points -= 1;
                            else if (plantIndexDrag == 1 || plantIndexDrag == 2) points -= 2;
                            else points -= 3;
                            isPlantPlaced[i][j] = true;
                            modelMatrix = glm::mat3(1);
                            modelMatrix *= transform2D::Translate(columns[plantOnGrassX] - plantOffset, rows[plantOnGrassY]);
                            plants.push_back(new Plant(meshes[plantNames[plantIndexDrag]], 
                                             plantDragMouseX,
                                             plantDragMouseY,
                                             plantIndexDrag, 
                                             i, j, 
                                             modelMatrix,
                                             plantIndexDrag));
                        }
                        break;
                    }
                }
            }
        }
        previousIndex = plantIndexDrag;
        plantIndexDrag = -1;
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
