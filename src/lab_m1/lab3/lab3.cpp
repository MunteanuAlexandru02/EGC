#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
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

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

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

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(0.25, 0.4, 0.33), true);
    AddMeshToList(square4);

    Mesh* tire_one = object2D::CreateSquare("tire_one", corner, squareSide, glm::vec3(1, 1, 1), true);
    AddMeshToList(tire_one);

    Mesh* tire_two = object2D::CreateSquare("tire_two", corner, squareSide, glm::vec3(0, 0, 0.25f), true);
    AddMeshToList(tire_two);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


bool basic = true;

bool scale_bool = true;

float Tx = 150.f;
float first_Tx = 100.f;
float second_Tx = 200.f;
float angular = 0;

void Lab3::Update(float deltaTimeSeconds)
{
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,t
    
    // add them over there!
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(Tx, 500.f);

    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(1.5, 1);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(first_Tx, 445.f);

    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(0.2, 0.2);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(angular);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["tire_one"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(second_Tx, 445.f);

    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(0.2, 0.2);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(angular);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["tire_one"], shaders["VertexColor"], modelMatrix);

    /*Tx += 30 * deltaTimeSeconds;
    first_Tx += 30*deltaTimeSeconds;
    second_Tx += 30*deltaTimeSeconds;*/

    glm::ivec2 res = window->GetResolution();

    //cout << res.x << " " << tx << endl;
    if (basic) {
        /// Walking forward -- check border then add
        if (res.x > 2.6 * tx + deltaTimeSeconds * 100 || res.y > 2.6 * tx + deltaTimeSeconds * 100) {
            tx += deltaTimeSeconds * 100;
            //ty += deltaTimeSeconds * 100;
        }
        else {
            basic = false;
        }

    }
    else if (!basic) {
        if (originalTx < tx || originalTy < ty) {
            tx -= deltaTimeSeconds * 100;
            //ty -= deltaTimeSeconds * 100;
        }
        else {
            basic = true;
        }
    }

    cout << basic << endl;

   /* if (res.x > 1.2 * tx + deltaTimeSeconds * 100 && res.y > 1.2 * ty + deltaTimeSeconds * 100) {
        tx += deltaTimeSeconds * 100;
        ty += deltaTimeSeconds * 100;

   }*/
    
    
        // Translation
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tx, ty);
        // TODO(student): Create animations by multiplying the current
        // transform matrix with the matrices you just implemented.
        // Remember, the last matrix in the chain will take effect first!
    
       
    
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);



    //Rotation

    angularStep += 20*deltaTimeSeconds;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 250);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 250);


    if (scale_bool) {
        scaleX += deltaTimeSeconds / 2.0;
        if (scaleX > 2.5)
            scale_bool = false;
    }
    else {
        scaleX -= deltaTimeSeconds / 2.0;
        if (scaleX < 1.0)
            scale_bool = true;
    }


    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(scaleX, scaleX);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);


    
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D)) {
        Tx += 600 * deltaTime;
        first_Tx += 600 * deltaTime;
        second_Tx += 600 * deltaTime;
        angular -= 50 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        Tx -= 600 * deltaTime;
        first_Tx -= 600 * deltaTime;
        second_Tx -= 600 * deltaTime;
        angular += 50 * deltaTime;
    }
    
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
