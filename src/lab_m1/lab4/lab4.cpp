#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Mesh* mesh2 = new Mesh("tank");
    mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "corp.obj");
    meshes[mesh2->GetMeshID()] = mesh2;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::vec3 color(1, 0, 0);
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

   // glm::vec3 color2(1, 0, 0);
    vector<VertexFormat> ver = { {{0,0,0}, color}, {{1, 1, 1}, color} };

    vector<unsigned int> indx = { 0, 1 };
    Mesh* line = new Mesh("generatedLine");
    line->SetDrawMode(GL_LINES);
    line->InitFromData(ver, indx);

    AddMeshToList(line);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawLine(glm::vec3 a, glm::vec3 b) {
    ;
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    glm::vec4 p = modelMatrix * glm::vec4(0, 0, 0, 1);
    glm::vec3 res = glm::vec3(p.x, p.y, p.z) - glm::vec3(a_x, a_y, a_z);
    //cout << res << endl;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(a_x, a_y, a_z) * transform3D::Scale(res.x, res.y, res.z);
    //modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    //modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    // modelMatrix *= transform3D::Scale(p.x, p.y, p.z);
    RenderMesh(meshes["generatedLine"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.0, 1.0, -3.0);
    RenderMesh(meshes["tank"], shaders["Simple"], modelMatrix);

    
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    RenderScene();
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W)) {
        translateZ -= deltaTime;
        scaleLineZ -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        translateZ += deltaTime;
        scaleLineZ += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D))
        translateX += deltaTime, scaleLineX += deltaTime;
    if (window->KeyHold(GLFW_KEY_A))
        translateX -= deltaTime, scaleLineX -= deltaTime;
    if (window->KeyHold(GLFW_KEY_R))
        translateY += deltaTime, scaleLineY += deltaTime;
    if (window->KeyHold(GLFW_KEY_F))
        translateY -= deltaTime, scaleLineY -= deltaTime;
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime;
        scaleY += deltaTime;
        scaleZ += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime;
        scaleY -= deltaTime;
        scaleZ -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_3)) angularStepOX += deltaTime;
    if (window->KeyHold(GLFW_KEY_4)) angularStepOX -= deltaTime;
    if (window->KeyHold(GLFW_KEY_5)) angularStepOY += deltaTime;
    if (window->KeyHold(GLFW_KEY_6)) angularStepOY -= deltaTime;
    if (window->KeyHold(GLFW_KEY_7)) angularStepOZ += deltaTime;
    if (window->KeyHold(GLFW_KEY_8)) angularStepOZ -= deltaTime;

    if (window->KeyHold(GLFW_KEY_I)) miniViewportArea.y += 2;
    if (window->KeyHold(GLFW_KEY_J)) miniViewportArea.y -= 2;
    if (window->KeyHold(GLFW_KEY_K)) miniViewportArea.x += 2;
    if (window->KeyHold(GLFW_KEY_L)) miniViewportArea.x -= 2;
    if (window->KeyHold(GLFW_KEY_U)) {
        miniViewportArea.height += 2;
        miniViewportArea.width += 2;
    }
    if (window->KeyHold(GLFW_KEY_O)) {
        miniViewportArea.height -= 2;
        miniViewportArea.width -= 2;
    }


}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    
    // TODO(student): Add viewport movement and scaling logic
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
