#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Enemy.h"
#include "lab_m1/Tema1/Star.h"
#include "lab_m1/Tema1/Plant.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
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

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        // TODO(student): If you need any other class variables, define them here.

        int lifeCounter = 3;
        int points = 5;
        int starMaxSize = 35;
        int sideSquare = 100;

        int plantIndexDrag = -1;
        int previousIndex = -1;
        int plantDragX;
        int plantDragY;
        int plantDragMouseX;
        int plantDragMouseY;

        int plantOnGrassX = -1;
        int plantOnGrassY;
        int plantOffset = 30;

        float tx{ 150.f }, ty{ 400.f };
        float originalTx{ 150.f }, originalTy{ 250.f };
        bool border{ false };

        float barrierX = 50.f, barrierY = 40.f;

        glm::vec3 columns = glm::vec3(100.f, 240.f, 380.f);
        glm::vec3 rows = glm::vec3(40.f, 180.f, 320.f);

        float gridPlantY{ 600.f };
        glm::vec4 gridPlantX = glm::vec4(100.f, 240.f, 380.f, 520.f);

        float hearthY{ 600.f };
        glm::vec3 hearthX = glm::vec3(720.f, 860.f, 1000.f);

        float startCostStar;

        std::vector<std::string> plantNames = { "orange_plant", "blue_plant", "yellow_plant", "purple_plant" };
        std::vector<std::string> enemyNames = { "orange_enemy", "blue_enemy", "yellow_enemy", "purple_enemy" };
        std::vector<std::string> projectileNames = { "orangeStar", "blueStar", "yellowStar", "purpleStar" };
        
        bool basic = true;
        bool scale_bool = true;

        std::vector<Enemy*> enemies;
        std::vector<Star*> stars;
        std::vector<Star*> projectile;
        std::vector<Plant*> plants;

        bool isPlantPlaced[3][3] = { false };

        float seconds{ 0.0 };
        float halfSquare{ 50.f };
        float enemyStartingPos{ 1350.f };

        float X = 1.f, Y = 1.f;

        float Tx = 150.f;
        float first_Tx = 100.f;
        float second_Tx = 200.f;
        float angular = 0;

    };
}   // namespace m1
