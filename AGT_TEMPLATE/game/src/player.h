#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class Player {

public:
    Player();
    ~Player();
    void initialise(engine::ref<engine::game_object> object);
    void onUpdate(const engine::timestep& timeStep);
    engine::ref<engine::game_object> object() const { return mObject; }
    void turn(float angle);
    void updateCamera3rdPerson(engine::perspective_camera& camera, const engine::timestep& timeStep);
    void update1stPersonCamera(engine::perspective_camera& camera, const engine::timestep& timestep);
    void updateCamera(engine::perspective_camera& camera, const engine::timestep& timestep);
    void jump();
    

private:
    float mSpeed{0.f};
    float mJumpTimer;
    float cameraRadius = 5.f;
    bool firstPerson = false; //
    bool canTransition = true;
    float mTransitionCameraTimer;
    engine::ref<engine::game_object> mObject;
    glm::vec3 cameraFront = glm::vec3(0, 0, 0);
    float height = 1.f;
    glm::vec3 prevFront = glm::vec3(0, 0, 0);
    std::pair<float, float> prevMousePosition;
};
