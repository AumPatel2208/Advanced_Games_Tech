#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class Player {

public:
    Player();
    ~Player();
    void initialise(engine::ref<engine::game_object> object);
    void moveIntoLevel1();
    void onUpdate(const engine::timestep& timestep);
    engine::ref<engine::game_object> object() const { return mObject; }
    void turn(float angle) const;
    void updateCamera3rdPerson(engine::perspective_camera& camera, const engine::timestep& timeStep);
    void update1stPersonCamera(engine::perspective_camera& camera, const engine::timestep& timestep);
    void menuUpdateCamera(engine::perspective_camera& camera, const engine::timestep& timestep, bool hasStarted);
    void updateCamera(engine::perspective_camera& camera, const engine::timestep& timestep);
    void jump();
    void setHasStarted(bool _hasStarted);


private:
    float mSpeed{0.f};
    float mJumpTimer;
    bool hasStarted = false;

    engine::ref<engine::game_object> mObject;
    glm::vec3 cameraFront = glm::vec3(0, 0, 0);
    float height = 1.f;
    glm::vec3 prevFront = glm::vec3(0, 0, 0);
    std::pair<float, float> prevMousePosition;
    std::pair<float, float> currentMousePosition;

    bool firstPerson = false; //
    bool canTransition = true;
    float mTransitionCameraTimer = 0.f;
    float camLookAtY = 0.f;
    float camPosBehind = 0.f;
    const float cameraPosYLimit = 4.f;
    const float cameraPosBehindLimit = 5.f;
    float ySensitivity3rdPerson = 2.f;
    float ySensitivity1stPerson = 5.f;


};
