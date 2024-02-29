#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);
    Mesh* CreateStar(const std::string& name, float bigRadius, float smallRadius, glm::vec3 color, bool fill = false);
    Mesh* CreatePlant(const std::string& name, glm::vec3 color, bool fill = false);
    Mesh* CreateEnemy(const std::string& name, float bigRadius, float smallRadius, glm::vec3 bigColor, glm::vec3 smallColor, bool fill = false);
}
