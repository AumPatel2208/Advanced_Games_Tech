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
    void updateCameraOld(engine::perspective_camera& camera, const engine::timestep& timeStep);
    void updateCamera(engine::perspective_camera& camera);
    void jump();
    

private:
    float mSpeed{0.f};
    float mTimer;
    float cameraRadius = 5.f;
    engine::ref<engine::game_object> mObject;
};
