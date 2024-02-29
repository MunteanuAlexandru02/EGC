#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/trans3D.h"
#include "lab_m1/Tema2/EnemyTank.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>


#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;
    glm::ivec2 resolution = window->GetResolution();



    camera = new implementedcamera::Camera();
    camera->Set(glm::vec3(0, 1, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 0.1, 0));

    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
    {
        Mesh* mesh = new Mesh("bodyTank");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senileTank");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senile.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turelaTank");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tunTank");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("house");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("HomeworkShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
            lightPosition = glm::vec3(0, 1, 1);
            lightPosition2 = glm::vec3(2, 1, 3);
            lightDirection = glm::vec3(0, -1, 0);
            materialShininess = 30;
            materialKd = 0.5;
            materialKs = 0.5;

            light_type = 0;
            angleOX = 0.f;
            angleOY = 0.f;
            angleLight = 40.f;
    }

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);

    //Place houses so that they don't collide with the main tank
    for (int i{ 0 }; i < HOUSE_NO; i++) {
        int xValue = rand() % 25;
        int zValue = rand() % 25;
        if (abs(xValue - (int)translate.x) < 3 || abs(zValue - (int)translate.z) < 3) {
            i--;
        }
        else {
            int x = rand() % 3;
            if (x == 0) housePosition.push_back(glm::vec3(xValue * 1.0, 0.5, zValue * 1.0));
            else if (x == 1) housePosition.push_back(glm::vec3(-xValue * 1.0, 0.5, zValue * 1.0));
            else if (x == 2) housePosition.push_back(glm::vec3(xValue * 1.0, 0.5, -zValue * 1.0));
            else if (x == 3) housePosition.push_back(glm::vec3(-xValue * 1.0, 0.5, -zValue * 1.0));
            x = rand() % 5 + 1;
            houseScale.push_back(glm::vec3(x, rand() % 7 + 1, x));
        }
    }
    //Place enemy tanks so that they avoid any collisions
    for (int i{0}; i < ENEMY_TANKS_NO; i++) {
        int houseIndex = rand() % HOUSE_NO;
        //  I will spawn a tank near a house so, in that way I will create an effect of
        //  pseudo-randomness
        if (hasTankNear[houseIndex] == false) {
            int hpx = housePosition[houseIndex].x + 5.f;
            int hpz = housePosition[houseIndex].z + 5.f;
            enemy[i] = new EnemyTank(glm::vec3(hpx, 1.3, hpz));
        }
    }
    
    text = new gfxc::TextRenderer(".", 1280, 720);
    text->Load("assets\\fonts\\Hack-Bold.ttf", 30);
}
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, int life)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int lifePoints = glGetUniformLocation(shader->program, "life");
    glUniform1i(lifePoints, life);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_pos = glGetUniformLocation(shader->program, "light_position2");
    glUniform3f(light_pos, lightPosition2.x, lightPosition2.y, lightPosition2.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    int type = glGetUniformLocation(shader->program, "lightType");
    glUniform1i(type, light_type);

    int angle = glGetUniformLocation(shader->program, "angle");
    glUniform1f(angle, angleLight);
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    //glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::FrameStart() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema2::CheckEnemyCollisions() {
    // Collisions between houses and enemy tanks
    for (int i = { 0 }; i < ENEMY_TANKS_NO; i++) {
        for (int j{ 0 }; j < HOUSE_NO; j++) {
            float boxMinX = housePosition[j].x - houseScale[j].x / 2.0;
            float boxMaxX = housePosition[j].x + houseScale[j].x / 2.0;
            float boxMinZ = housePosition[j].z - houseScale[j].z / 2.0;
            float boxMaxZ = housePosition[j].z + houseScale[j].z / 2.0;
            float boxMinY = housePosition[j].y + houseScale[j].y / 2.0;
            float boxMaxY = housePosition[j].y - houseScale[j].y / 2.0;

            float x = fmax(boxMinX, fmin(enemy[i]->enemyPosition.x / 2.0, boxMaxX));
            float y = fmax(boxMinY, fmin(enemy[i]->enemyPosition.y / 2.0, boxMaxY));
            float z = fmax(boxMinZ, fmin(enemy[i]->enemyPosition.z / 2.0, boxMaxZ));

            float distance = sqrt((x - enemy[i]->enemyPosition.x / 2.0) * (x - enemy[i]->enemyPosition.x / 2.0)
                + (z - enemy[i]->enemyPosition.z / 2.0) * (z - enemy[i]->enemyPosition.z / 2.0));

            float xRadius = TANK_RADIUS;
            if (xRadius > distance) {
                glm::vec3 idk1 = glm::vec3(enemy[i]->enemyPosition.x / 2.0, 0, enemy[i]->enemyPosition.z / 2.0);
                glm::vec3 idk2 = glm::vec3(housePosition[j].x * houseScale[j].x / 2.0,
                    0,
                    housePosition[j].z * houseScale[j].z / 2.0);
                centreDifference = idk1 - idk2;
                float P = xRadius - distance;
                normalizedCentre = glm::normalize(centreDifference);
                enemy[i]->enemyPosition.x += abs(P) * normalizedCentre.x * 0.5f;
                enemy[i]->enemyPosition.z += abs(P) * normalizedCentre.z * 0.5f;
            }
        }
    }

    for (int i{ 0 }; i < ENEMY_TANKS_NO; i++) {
        for (int j{ 0 }; j < ENEMY_TANKS_NO; j++) {
            if (i != j) {
                float distance = glm::distance(glm::vec2(enemy[j]->enemyPosition.x / 2.0, enemy[j]->enemyPosition.z / 2.0),
                                                        glm::vec2(enemy[i]->enemyPosition.x / 2.0, enemy[i]->enemyPosition.z / 2.0));
                float xRadius = 2 * TANK_RADIUS + TANK_OFFSET_CONSTANT;
                float zRadius = 2 * TANK_RADIUS + TANK_OFFSET_CONSTANT;
                if (xRadius > distance || zRadius > distance) {
                    centreDifference = enemy[j]->enemyPosition / glm::vec3(2.0, 2.0, 2.0) - enemy[i]->enemyPosition / glm::vec3(2.0, 2.0, 2.0);

                    float P = xRadius - distance;
                    normalizedCentre = glm::normalize(centreDifference);
                    enemy[j]->enemyPosition.x += abs(P) * normalizedCentre.x * HOUSE_COLLISION_CONSTANT;
                    enemy[j]->enemyPosition.z += abs(P) * normalizedCentre.z * HOUSE_COLLISION_CONSTANT;

                    enemy[i]->enemyPosition.x -= abs(P) * normalizedCentre.x * HOUSE_COLLISION_CONSTANT;
                    enemy[i]->enemyPosition.z -= abs(P) * normalizedCentre.z * HOUSE_COLLISION_CONSTANT;
                }
            }
        }
    }
}

void Tema2::CheckProjectileCollisions() {
    for (int i{ 0 }; i < projectiles.size(); i++) {
        for (int j{ 0 }; j < HOUSE_NO; j++) {
            float distance = glm::distance(glm::vec2(projectiles[i]->projectilePosition.x / 2.0, projectiles[i]->projectilePosition.z / 2.0),
                                            glm::vec2(housePosition[j].x, housePosition[j].z));

            float xRadius = 0.08 + houseScale[j].x / 2.0;
            float zRadius = 0.08 + houseScale[j].z / 2.0;

            if (xRadius > distance || zRadius > distance) {
                projectiles.erase(projectiles.begin() + i, projectiles.begin() + i + 1);
                break;
            }
        }
    }

    for (int i{ 0 }; i < projectiles.size(); i++) {
        for (int j{ 0 }; j < ENEMY_TANKS_NO; j++) {
            float distance = glm::distance(glm::vec2(projectiles[i]->projectilePosition.x, projectiles[i]->projectilePosition.z),
                glm::vec2(enemy[j]->enemyPosition.x / 2.0, enemy[j]->enemyPosition.z / 2.0));

            float xRadius =  1;
            float zRadius =  1;

            if (xRadius > distance || zRadius > distance) {
                if (enemy[j]->health > 0) {
                    if (enemy[j]->health == 1)
                        score += 200;
                    else score += 100;
                    enemy[j]->health--;
                }
                projectiles.erase(projectiles.begin() + i, projectiles.begin() + i + 1);
                break;
            }
        }
    }
    for (int i{ 0 }; i < projectiles.size(); i++) {
        if (projectiles[i]->projectilePosition.y < 0)
            projectiles.erase(projectiles.begin() + i, projectiles.begin() + i + 1);
    }

    for (int i{ 0 }; i < projectiles.size(); i++) {
        float distance = glm::distance(glm::vec2(projectiles[i]->projectilePosition.x, projectiles[i]->projectilePosition.z),
            glm::vec2(translate.x / 2.0, translate.z / 2.0));

        float xRadius = TANK_RADIUS;
        float zRadius = TANK_RADIUS;

        if (xRadius > distance || zRadius > distance) {;
            projectiles.erase(projectiles.begin() + i, projectiles.begin() + i + 1);
            mainTankHealth--;
            break;
        }
    }

}

bool Tema2::CheckTankOutMap() {
    if (translate.x > 55.0 || translate.x < -55.f || translate.z > 55.f || translate.z < -55.f)
        return true;
    return false;
}

void Tema2::CheckAndPrepareAttack(int i) {
    if (translate.x - enemy[i]->enemyPosition.x < 7 || translate.z - enemy[i]->enemyPosition.z < 7) {
        angleToTank = atan2((enemy[i]->enemyPosition.x - translate.x), (enemy[i]->enemyPosition.z - translate.z));
        enemy[i]->turretRotation = angleToTank;   
    }
}

void Tema2::Attack(int i) {
    if (enemy[i]->health > 0) {
        if (translate.x - enemy[i]->enemyPosition.x < 2.f || translate.z - enemy[i]->enemyPosition.z < 2.f) {
            int val = seconds * 100;
            float checkCooldown = val * 1.0 / 100;
            if (checkCooldown > 3.0 + enemy[i]->lastShot) {
                float x = enemy[i]->enemyPosition.x / 2.0;
                float z = enemy[i]->enemyPosition.z / 2.0;
                projectiles.push_back(new Projectile(glm::vec3(x - 1.4 * sin(enemy[i]->turretRotation) ,
                    translate.y - 0.8, 
                    z - 1.4 * cos(enemy[i]->turretRotation)),
                    glm::vec3(PROJECTILE_SCALE, PROJECTILE_SCALE, PROJECTILE_SCALE),
                    enemy[i]->turretRotation));
                enemy[i]->lastShot = checkCooldown;
            }
        }
    }
}

void Tema2::RenderScene()
{
    if (translate.y < -150.f) {
        cout << "You went out of bounds!";
        exit(1);

    }
    ok = CheckTankOutMap();
    for (int i = 0; i < projectiles.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= trans3D::Translate(projectiles[i]->projectilePosition.x - 1.4 * sin(projectiles[i]->turretRotation),
            projectiles[i]->projectilePosition.y,
            projectiles[i]->projectilePosition.z - 1.4 * cos(projectiles[i]->turretRotation));
        modelMatrix *= trans3D::Scale(projectiles[i]->projectileScale.x,
            projectiles[i]->projectileScale.y,
            projectiles[i]->projectileScale.z);
        RenderSimpleMesh(meshes["projectile"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.85, 0.8248, 0.0935));
    }
    camera->Follow(translate / glm::vec3(2, 2, 2));

    seconds = glfwGetTime();
    int integerSeconds = seconds;
    if (seconds > 90) {
     
        cout << "Your score is: " << score << endl;
        exit(1);

    }

    if (mainTankHealth == 0)
        stopMoving = true;

    if (stopMoving) {
        cout << "You lost\n";
        exit(1);
    }

    modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["plane"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.11,0.33,0.0429));

    for (int i{ 0 }; i < HOUSE_NO; i++) {
        modelMatrix = glm::mat4(1);
        float x = housePosition[i].x;
        float z = housePosition[i].z;
        modelMatrix *= trans3D::Translate(x, 0, z);
        modelMatrix *= trans3D::Scale(houseScale[i].x, houseScale[i].y, houseScale[i].z);
        RenderSimpleMesh(meshes["house"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.4116, 0.42, 0.4187));
    }

    for (int i{ 0 }; i < ENEMY_TANKS_NO; i++) {
        Attack(i);

        modelMatrix = glm::mat4(1);
        modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
        modelMatrix *= trans3D::Translate(enemy[i]->enemyPosition.x, enemy[i]->enemyPosition.y, enemy[i]->enemyPosition.z);
        modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
        modelMatrix *= trans3D::RotateOY(enemy[i]->angularStep.y);
        RenderSimpleMesh(meshes["senileTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.0174, 0.58, 0.3737), enemy[i]->health);

        modelMatrix = glm::mat4(1);
        modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
        modelMatrix *= trans3D::Translate(enemy[i]->enemyPosition.x, enemy[i]->enemyPosition.y, enemy[i]->enemyPosition.z);
        modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
        modelMatrix *= trans3D::RotateOY(enemy[i]->angularStep.y);
        RenderSimpleMesh(meshes["bodyTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.0174, 0.58, 0.3737), enemy[i]->health);

        modelMatrix = glm::mat4(1);
        modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
        modelMatrix *= trans3D::Translate(enemy[i]->enemyPosition.x, enemy[i]->enemyPosition.y, enemy[i]->enemyPosition.z);
        modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
        //modelMatrix *= trans3D::RotateOY(enemy[i]->angularStep.y);
        modelMatrix *= trans3D::RotateOY(enemy[i]->turretRotation);
        RenderSimpleMesh(meshes["turelaTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.0174, 0.58, 0.3737), enemy[i]->health);

        modelMatrix = glm::mat4(1);
        modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
        modelMatrix *= trans3D::Translate(enemy[i]->enemyPosition.x, enemy[i]->enemyPosition.y, enemy[i]->enemyPosition.z);
        modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
        //modelMatrix *= trans3D::RotateOY(enemy[i]->angularStep.y);
        modelMatrix *= trans3D::RotateOY(enemy[i]->turretRotation);
        RenderSimpleMesh(meshes["tunTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.0174, 0.58, 0.3737), enemy[i]->health);
    }

    modelMatrix = glm::mat4(1);
    modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
    modelMatrix *= trans3D::Translate(translate.x, translate.y, translate.z);
    modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
    modelMatrix *= trans3D::RotateOY(angularStep.y);
    RenderSimpleMesh(meshes["senileTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.94, 0.9337, 0.752), mainTankHealth);

    modelMatrix = glm::mat4(1);
    modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
    modelMatrix *= trans3D::Translate(translate.x, translate.y, translate.z);
    modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
    modelMatrix *= trans3D::RotateOY(angularStep.y);
    RenderSimpleMesh(meshes["bodyTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.94, 0.9337, 0.752), mainTankHealth);

    modelMatrix = glm::mat4(1);
    modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
    modelMatrix *= trans3D::Translate(translate.x, translate.y, translate.z);
    modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
    modelMatrix *= trans3D::RotateOY(turretRotation);
    RenderSimpleMesh(meshes["turelaTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.94, 0.9337, 0.752), mainTankHealth);

    modelMatrix = glm::mat4(1);
    modelMatrix *= trans3D::Scale(TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE);
    modelMatrix *= trans3D::Translate(translate.x, translate.y, translate.z);
    modelMatrix *= trans3D::RotateOY(OY_ROTATION_CONSTANT);
    modelMatrix *= trans3D::RotateOY(turretRotation);

    modelMatrix *= trans3D::Translate(NEGATIVE_TURRET_PIVOT,
        NEGATIVE_TURRET_PIVOT,
        NEGATIVE_TURRET_PIVOT);

    if (cannonRotation < MAX_VALUE_DOWN_TURRET)
        modelMatrix *= trans3D::RotateOZ(cannonRotation);
    else
        modelMatrix *= trans3D::RotateOZ(MAX_VALUE_DOWN_TURRET);

    modelMatrix *= trans3D::Translate(POSITIVE_TURRET_PIVOT,
        POSITIVE_TURRET_PIVOT,
        POSITIVE_TURRET_PIVOT);

    RenderSimpleMesh(meshes["tunTank"], shaders["HomeworkShader"], modelMatrix, glm::vec3(0.94, 0.9337, 0.752), mainTankHealth);

    /*check collisions for main tank*/
    for (int j{ 0 }; j < HOUSE_NO; j++) {
        float boxMinX = housePosition[j].x - houseScale[j].x / 2.0;
        float boxMaxX = housePosition[j].x + houseScale[j].x / 2.0;
        float boxMinZ = housePosition[j].z - houseScale[j].z / 2.0;
        float boxMaxZ = housePosition[j].z + houseScale[j].z / 2.0;
        float boxMinY = housePosition[j].y + houseScale[j].y / 2.0;
        float boxMaxY = housePosition[j].y - houseScale[j].y / 2.0;

        float x = fmax(boxMinX, fmin(translate.x / 2.0, boxMaxX));
        float y = fmax(boxMinY, fmin(translate.y / 2.0, boxMaxY));
        float z = fmax(boxMinZ, fmin(translate.z / 2.0, boxMaxZ));

        float distance = sqrt((x - translate.x / 2.0) * (x - translate.x / 2.0)
            + (z - translate.z / 2.0) * (z - translate.z / 2.0));
//                            + (y - translate.y / 2.0) * (y - translate.y / 2.0));

        float xRadius = TANK_RADIUS;// + houseScale[j].x / 2.0;
        if (xRadius > distance) {
            glm::vec3 idk1 = glm::vec3(translate.x / 2.0, 0, translate.z / 2.0);
            glm::vec3 idk2 = glm::vec3(housePosition[j].x * houseScale[j].x / 2.0,
                                       0, 
                                       housePosition[j].z * houseScale[j].z / 2.0);
            //centreDifference = translate / glm::vec3(2.0, 2.0, 2.0) - housePosition[j] * houseScale[j];
            centreDifference = idk1 - idk2;
            float P = xRadius - distance;
            normalizedCentre = glm::normalize(centreDifference);
            translate.x += abs(P) * normalizedCentre.x * 0.5f;
            translate.z += abs(P) * normalizedCentre.z * 0.5f;
        }
    }

    CheckEnemyCollisions();
    CheckProjectileCollisions();


    for (int i{ 0 }; i < ENEMY_TANKS_NO; i++) {
        float distance = glm::distance(glm::vec2(translate.x / 2.0, translate.z / 2.0), glm::vec2(enemy[i]->enemyPosition.x / 2.0, enemy[i]->enemyPosition.z / 2.0));
        float xRadius = 2 * TANK_RADIUS + TANK_OFFSET_CONSTANT;
        float zRadius = 2 * TANK_RADIUS + TANK_OFFSET_CONSTANT;
        if (xRadius > distance || zRadius > distance) {
            centreDifference = translate / glm::vec3(2.0, 2.0, 2.0) - enemy[i]->enemyPosition / glm::vec3(2.0, 2.0, 2.0);

            float P = xRadius - distance;

            normalizedCentre = glm::normalize(centreDifference);

            translate.x += abs(P) * normalizedCentre.x;// *HOUSE_COLLISION_CONSTANT;
            translate.z += abs(P) * normalizedCentre.z;// *HOUSE_COLLISION_CONSTANT;

            enemy[i]->enemyPosition.x -= abs(P) * normalizedCentre.x;// *HOUSE_COLLISION_CONSTANT;
            enemy[i]->enemyPosition.z -= abs(P) * normalizedCentre.z;// *HOUSE_COLLISION_CONSTANT;
        }
    }
    //Need to change the state;
    if (integerSeconds % 3 == 0) {
        for (int i{ 0 }; i < ENEMY_TANKS_NO; i++) {
            int value = rand() % 4;
            switch (value) {
            case 0:
                enemy[i]->state = FORWARD;
                break;
            case 1:
                enemy[i]->state = BACKWARD;
                break;
            case 2:
                enemy[i]->state = ROTATE_LEFT;
                break;
            case 3:
                enemy[i]->state = ROTATE_RIGHT;
                break;
            default:
                break;
            }
        }
    }
    //}
}

void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
   // glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    RenderScene();
    //DrawCoordinateSystem();

    stringstream ss;
    ss << "Score: ";
    ss << score;
    text->RenderText(ss.str(), 0, 0, 1, glm::vec3(1));
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    viewMatrix = glm::lookAt(elevation, centerScreen, downward);
    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar) * viewMatrix;
    
    RenderScene();
   // DrawCoordinateSystem();

    if (!stopMoving) {
        for (int i{ 0 }; i < ENEMY_TANKS_NO; i++) {
            if (enemy[i]->state == FORWARD && enemy[i]->health > 0) {
                enemy[i]->enemyPosition.z -= 5 * deltaTimeSeconds * cos(enemy[i]->angularStep.y);
                enemy[i]->enemyPosition.x -= 5 * deltaTimeSeconds * sin(enemy[i]->angularStep.y);
            }
            if (enemy[i]->state == BACKWARD && enemy[i]->health > 0) {
                enemy[i]->enemyPosition.z += 5 * deltaTimeSeconds * cos(enemy[i]->angularStep.y);
                enemy[i]->enemyPosition.x += 5 * deltaTimeSeconds * sin(enemy[i]->angularStep.y);

            }
            if (enemy[i]->state == ROTATE_LEFT && enemy[i]->health > 0) {
                enemy[i]->angularStep.y += deltaTimeSeconds;
                //camera->RotateThirdPerson_OX(0.001 * -angularStep.y);
                //camera->RotateThirdPerson_OY(deltaTimeSeconds);
                enemy[i]->turretRotation += deltaTimeSeconds;

            }
            if (enemy[i]->state == ROTATE_RIGHT && enemy[i]->health > 0) {
                enemy[i]->angularStep.y -= deltaTimeSeconds;
                //camera->RotateThirdPerson_OY(-deltaTime);
                enemy[i]->turretRotation -= deltaTimeSeconds;
            }
            if (enemy[i]->health > 0) CheckAndPrepareAttack(i);
        }

        for (int i{ 0 }; i < projectiles.size(); i++) {
            projectiles[i]->projectilePosition.z -= 15 * deltaTimeSeconds * cos(projectiles[i]->turretRotation);
            projectiles[i]->projectilePosition.y -= GRAVITY_CONSTANT * deltaTimeSeconds;
            projectiles[i]->projectilePosition.x -= 15 * deltaTimeSeconds * sin(projectiles[i]->turretRotation);
        }
    }

    
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(8 * deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(8 * -deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(8* -deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(8* deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(8*-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(8*deltaTime * cameraSpeed);
        }
    }
    else if (!ok && !stopMoving){
        if (window->KeyHold(GLFW_KEY_W)) {
            translate.z -= 10 * deltaTime * cos(angularStep.y);
            translate.x -= 10 * deltaTime * sin(angularStep.y);
            position.z -= deltaTime;
            position.x -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            translate.z += 5 * deltaTime * cos(angularStep.y);
            translate.x += 5 * deltaTime * sin(angularStep.y);
            position.z += deltaTime;
            position.x += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            angularStep.y += deltaTime;
            //camera->RotateThirdPerson_OX(0.001 * -angularStep.y);
            camera->RotateThirdPerson_OY(deltaTime);
            turretRotation += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            angularStep.y -= deltaTime;
            camera->RotateThirdPerson_OY(-deltaTime);
            turretRotation -= deltaTime;
        }
    }
    else if (ok) {
        translate.y -= 30 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_M)) {
        fov += 5.f;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    }
    if (window->KeyHold(GLFW_KEY_N)) {
        fov -= 5.f;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    }


    if (window->KeyHold(GLFW_KEY_B)) {
        left -= 3.f;
        right += 3.f;
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (window->KeyHold(GLFW_KEY_C)) {
        left += 3.f;
        right -= 3.f;
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (window->KeyHold(GLFW_KEY_V)) {
        bottom -= 3.f;
        top += 3.f;
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (window->KeyHold(GLFW_KEY_X)) {
        bottom += 3.f;
        top -= 3.f;
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O)
    {
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    if (key == GLFW_KEY_P)
    {
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    }

    if (key == GLFW_KEY_L) {
        camera->RotateFirstPerson_OZ(0.2);
    }
    if (key == GLFW_KEY_K) {
        camera->RotateFirstPerson_OX(-0.2);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            if (!window->GetSpecialKeyState()) {
                renderCameraTarget = false;
                camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
                camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
            }
        }

        //if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {

            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            //if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
              //  renderCameraTarget = true;
                
            //}
        //}
    }
    else if (!stopMoving){
        turretRotation -= deltaX * TURRET_SLOWING;
        cannonRotation += deltaY * TURRET_SLOWING;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    int val = seconds * 100;
    float checkCooldown = val * 3 / 100;
    if (button == 1 && checkCooldown > 3 + cooldown) {
        projectiles.push_back(new Projectile(glm::vec3(translate.x / 2.0 - 0.7 * sin(turretRotation),
                                             translate.y - 0.8, 
                                             translate.z / 2.0 - 0.7 * cos(turretRotation)),
                                             glm::vec3(PROJECTILE_SCALE, PROJECTILE_SCALE, PROJECTILE_SCALE),
                                             turretRotation));
        cooldown = checkCooldown;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
    text = new gfxc::TextRenderer(".", width, height);
    text->Load("assets\\fonts\\Hack-Bold.ttf", 30);
}
