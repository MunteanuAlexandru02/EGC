#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0, -1), color),
        VertexFormat(corner + glm::vec3(length, 0, -1), color),
        VertexFormat(corner + glm::vec3(length, length, -1), color),
        VertexFormat(corner + glm::vec3(0, length, -1), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0, -1), color),
        VertexFormat(corner + glm::vec3(length, 0, -1), color),
        VertexFormat(corner + glm::vec3(length, width, -1), color),
        VertexFormat(corner + glm::vec3(0, width, -1), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateStar(
    const std::string& name,
    float bigRadius,
    float smallRadius,
    glm::vec3 color,
    bool fill
)
{

    float PI = 3.14159;
    float c1 = cos(2 * PI / 5.0);
    float c2 = cos(PI / 5.0);
    float s1 = sin(2 * PI / 5.0);
    float s2 = sin(4 * PI / 5.0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(0, -smallRadius, 0), color),
        VertexFormat(glm::vec3(smallRadius * s1, -smallRadius * c1, 0), color),
        VertexFormat(glm::vec3(smallRadius * s2, smallRadius * c2, 0), color),
        VertexFormat(glm::vec3(-smallRadius * s2, smallRadius * c2, 0), color),
        VertexFormat(glm::vec3(-smallRadius * s1, -smallRadius * c1, 0), color),

        //Coordinates for the bigger penthagon
        VertexFormat(glm::vec3(0, bigRadius, 0), color),
        VertexFormat(glm::vec3(bigRadius * s1, bigRadius * c1, 0), color),
        VertexFormat(glm::vec3(bigRadius * s2, -bigRadius * c2, 0), color),
        VertexFormat(glm::vec3(-bigRadius * s2, -bigRadius * c2, 0), color),
        VertexFormat(glm::vec3(-bigRadius * s1, bigRadius * c1, 0), color)
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = { 
                                            0, 1, 2,
                                            0, 2, 3,
                                            0, 3, 4,
                                            0, 4, 5,
                                            0, 5, 1,
                                            4, 3, 6,
                                            3, 2, 7,
                                            2, 1, 8,
                                            5, 1, 9,
                                            4, 10, 5

                                        };

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreatePlant(
    const std::string& name,
    glm::vec3 color,
    bool fill
)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(50, 50, -0.5), color),
        VertexFormat(glm::vec3(70, 0, -0.5), color),
        VertexFormat(glm::vec3(90, 50, -0.5), color),
        VertexFormat(glm::vec3(70, 100, -0.5), color),
        //Dreptunghi
        VertexFormat(glm::vec3(70, 75, -0.5), color),
        VertexFormat(glm::vec3(70, 25, -0.5), color),
        VertexFormat(glm::vec3(110, 25, -0.5), color),
        VertexFormat(glm::vec3(110, 75, -0.5), color)

    };

    Mesh* plant = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2,
                                          3, 0, 2,
                                          4, 5, 7, 
                                          6, 7, 5};

    if (!fill) {
        plant->SetDrawMode(GL_LINE_LOOP);
    }

    plant->InitFromData(vertices, indices);
    return plant;
}

Mesh* object2D::CreateEnemy(
    const std::string& name,
    float bigRadius,
    float smallRadius,
    glm::vec3 bigColor,
    glm::vec3 smallColor,
    bool fill
)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), bigColor),
        VertexFormat(glm::vec3(0, bigRadius, 0), bigColor),
        VertexFormat(glm::vec3(bigRadius * sqrt(3) / 2.0, bigRadius / 2.0, 0), bigColor),
        VertexFormat(glm::vec3(bigRadius * sqrt(3) / 2.0, -bigRadius / 2.0, 0), bigColor),
        VertexFormat(glm::vec3(0.f, -bigRadius, 0), bigColor),
        VertexFormat(glm::vec3(-bigRadius * sqrt(3) / 2.0, -bigRadius / 2.0, 0), bigColor),
        VertexFormat(glm::vec3(-bigRadius * sqrt(3) / 2.0, bigRadius / 2.0, 0), bigColor),
        
        VertexFormat(glm::vec3(0, 0, 0), smallColor),
        VertexFormat(glm::vec3(0, smallRadius, 0), smallColor),
        VertexFormat(glm::vec3(smallRadius * sqrt(3) / 2.0, smallRadius / 2.0, 0), smallColor),
        VertexFormat(glm::vec3(smallRadius * sqrt(3) / 2.0, -smallRadius / 2.0, 0), smallColor),
        VertexFormat(glm::vec3(0.f, -smallRadius, 0), smallColor),
        VertexFormat(glm::vec3(-smallRadius * sqrt(3) / 2.0, -smallRadius / 2.0, 0), smallColor),
        VertexFormat(glm::vec3(-smallRadius * sqrt(3) / 2.0, smallRadius / 2.0, 0), smallColor)

    };

    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices = {   
                                            7, 9, 8,
                                            7, 10, 9,
                                            7, 11, 10,
                                            7, 12, 11,
                                            7, 13, 12,
                                            7, 8, 13,
                                            0, 2, 1,
                                            0, 3, 2,
                                            0, 4, 3,
                                            0, 5, 4,
                                            0, 6, 5,
                                            0, 1, 6
                                            
    
    };

    enemy->InitFromData(vertices, indices);

    return enemy;
}