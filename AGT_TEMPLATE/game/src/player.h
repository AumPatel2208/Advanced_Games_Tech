#pragma once
#include <engine.h>

#include "AnimationHandler.h"
#include "glm/gtx/rotate_vector.hpp"

class Player {

public:
    Player();
    ~Player();
    void initialise();
    void moveIntoLevel1() const;
    void onUpdate(const engine::timestep& timestep);
    void renderHud(engine::ref<engine::text_manager>& textManager) const;
    void idle(engine::timestep timestep);
    void walk(const bool& forward, const engine::timestep& timestep);
    engine::ref<engine::game_object> object() const { return mObject; }
    void turn(float angle) const;
    void updateCamera3rdPerson(engine::perspective_camera& camera, const engine::timestep& timeStep);
    void update1stPersonCamera(engine::perspective_camera& camera, const engine::timestep& timestep) const;
    void menuUpdateCamera(engine::perspective_camera& camera, const engine::timestep& timestep, bool hasStarted);
    void updateCamera(engine::perspective_camera& camera, const engine::timestep& timestep);
    void jump();
    void setHasStarted(bool _hasStarted);


    bool getToInteractWithNpc() const { return toInteractWithNpc; }
    void setToInteractWithNpc(bool interactWithNpc) { this->toInteractWithNpc = interactWithNpc; }
    void setDialogueTimer(const float& timer) { mDialogueTimer = timer; }

private:
    float mSpeed{ 0.f };
    float mJumpTimer;
    float mMovementTimer;
    float mStaminaRecoveryTimer = 0.f;
    bool hasStarted = false;

    // HUD values
    int mHealthPoints = 100;// health points
    float mStamina = 100.f; // stamina points
    float mStaminaRecoverySpeed = 10.f;

    engine::ref<engine::game_object> mObject;
    glm::vec3 cameraFront = glm::vec3(0, 0, 0);
    float height = 1.f;
    glm::vec3 prevFront = glm::vec3(0, 0, 0);
    std::pair<float, float> prevMousePosition;
    std::pair<float, float> currentMousePosition;

    bool toInteractWithNpc = false;
    float mDialogueTimer = 0.f;

    bool firstPerson = false; //
    bool canTransition = true;
    float mTransitionCameraTimer = 0.f;
    float camLookAtY = 0.f;
    float camPosBehind = 0.f;
    const float cameraPosYLimit = 4.f;
    const float cameraPosBehindLimit = 5.f;
    float ySensitivity3rdPerson = 2.f;
    float ySensitivity1stPerson = 5.f;

    AnimationHandler animationHandler ={};

};
