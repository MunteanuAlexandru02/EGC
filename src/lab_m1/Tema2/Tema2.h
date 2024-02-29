#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/Tema2/tema_camera.h"
#include "lab_m1/Tema2/EnemyTank.h"
#include "lab_m1/Tema2/Projectile.h"

#include <vector>
#include "components/simple_scene.h"
#include "components/transform.h"

#define OY_ROTATION_CONSTANT        3.6
#define ROTATION_SPEED_CONSTANT     0.9
#define TURRET_SLOWING              0.0035
#define NEGATIVE_TURRET_PIVOT       -0.31
#define POSITIVE_TURRET_PIVOT       0.31
#define MAX_VALUE_DOWN_TURRET       0.504
#define TANK_DEFAULT_SCALE          0.5
#define HOUSE_NO                    30
#define ENEMY_TANKS_NO              3
#define TANK_RADIUS                 0.6
#define HOUSE_COLLISION_CONSTANT    0.5
#define TANK_OFFSET_CONSTANT        0.7
#define PROJECTILE_SCALE            0.08
#define GRAVITY_CONSTANT            0.1

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene();
        void CheckEnemyCollisions();
        void CheckProjectileCollisions();
        bool CheckTankOutMap();
        void CheckAndPrepareAttack(int i);
        void Attack(int i);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, int life = -1);

    protected:
        implementedcamera::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        bool renderCameraTarget;
        bool projectionType;
        float right{ 5.f }, left{ -5.f }, bottom{ -5.f }, top{ 5.f }, fov{ 60 };
        float zNear = 0.01f;
        float zFar = 200.f;
        glm::vec3 translate{ 0, 1.3, 0 };
        //glm::vec3 translate{ 1.f, 1.3, 1.f };
        glm::vec3 angularStep{ 0.0, 0.0, 0.0 };
        glm::vec3 position{ 1.f, 1.3, 1.f };

        float turretRotation{ 0.0 }, cannonRotation{ 0.0 };
        glm::vec3 centreDifference;
        glm::vec3 centreDifference2;

        glm::vec3 negative_values{ NEGATIVE_TURRET_PIVOT, NEGATIVE_TURRET_PIVOT, NEGATIVE_TURRET_PIVOT };
        glm::vec3 positive_values{ POSITIVE_TURRET_PIVOT, POSITIVE_TURRET_PIVOT, POSITIVE_TURRET_PIVOT };
        glm::vec3 default_scale_values{ TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE, TANK_DEFAULT_SCALE };
        glm::vec3 normalizedCentre;
        glm::vec3 normalizedCentre2;


        //glm::vec3 housePosition[HOUSE_NO];
        //glm::vec3 houseScale[HOUSE_NO];

        //glm::vec3 enemyPosition[ENEMY_TANKS_NO];

        std::vector<glm::vec3> housePosition;
        std::vector<glm::vec3> houseScale;
        //vector<glm::vec3> enemyPosition;

        /* 
            Created a diff class for enemyTanks because it will be easier to render their
            movement.
        */
        EnemyTank* enemy[ENEMY_TANKS_NO];
        std::vector<Projectile*> projectiles;

        bool hasTankNear[HOUSE_NO] = { false };
        glm::vec3 elevation{ 0.f, 10.f, 0.f };
        glm::vec3 centerScreen{ 0.f, 0.f, -2.f };
        glm::vec3 downward{ 0,0,-1 };
        float cooldown{ 0.0 };

        float seconds{ 0.0 };
        GLenum polygonMode;
        ViewportArea miniViewportArea;
        // TODO(student): If you need any other class variables, define them here.
        bool ok = false;
        float angleToTank;

        /*glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;sss
        float materialKs;
*/
        int mainTankHealth = 6;

        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        // TODO(student): If you need any other class variables, define them here.
        int light_type;
        float angleLight;
        float angleOY, angleOX;

        glm::vec3 lightPosition2;
        glm::vec3 lightDirection2;
        int score = 0;
        bool stopMoving = false;

        gfxc::TextRenderer* text;
    };
}   // namespace m1
