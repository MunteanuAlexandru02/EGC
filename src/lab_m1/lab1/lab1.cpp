#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!


}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;

    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;

    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}

float total_time = 0;
float height_diff = 0;
float bf_y;
void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(color_red, color_green, color_blue, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    if (jump == true) {
        total_time += deltaTimeSeconds;
        y = bf_y + speed * total_time - 5 * total_time * total_time;

        if (y <= initial_y) {
            jump = false;
        }
    }
    //else if (y > 0.5) { y -= 1.f * deltaTimeSeconds; }

    RenderMesh(meshes[name], glm::vec3(x, y, z));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

float globalDeltaTime;
void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`
       
    if (global_button == 2)
        return;
        if (window->KeyHold(GLFW_KEY_W)) {
            z -= 2.f * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_S)) {
            z += 2.f * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_D)) {
            x += 2.f * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_A)) {
            x -= 2.f * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_E)) {
            y += 2.f * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_Q)) {
            y -= 2.f * deltaTime;
        }
}

float GenerateRandomFloat() {
    return (float)(rand()) / (float)(rand());
}

float i = 0;

void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        color_red = GenerateRandomFloat();
        float floor_color_red = floor(color_red);
        color_red -= floor_color_red;

        color_blue = GenerateRandomFloat();
        float floor_color_blue = floor(color_blue);
        color_blue -= floor_color_blue;

        color_green = GenerateRandomFloat();
        float floor_color_green = floor(color_green);
        color_green -= floor_color_green;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_H) {
        if (counter_meshes == 0) {
            name = "sphere";
        }
        else if (counter_meshes == 1) {
            name = "box";
        }
        else if (counter_meshes == 2) {
            name = "teapot";
        }
        else {
            counter_meshes = -1;
        }
        
        counter_meshes++;
    }

    if (key == GLFW_KEY_SPACE) {
        total_time = 0.0;
        bf_y = y;
        jump = true;
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    global_button = button;
        
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    global_button = 0;
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
