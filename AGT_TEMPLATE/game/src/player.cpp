#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


Player::Player() {
    mJumpTimer = 0.f;
    mSpeed = 1.f;
}

Player::~Player() {}

void Player::initialise(engine::ref<engine::game_object> object) {
    mObject = object;
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));
    // mObject->set_forward(glm::vec3(0.f, 0.f, 1.f));
    // mObject->set_position(glm::vec3(0.f, 15.f, 0.f));
    // mObject->set_forward(glm::rotate(mObject->forward(), 180.f, glm::vec3(1.f, 0.f, 0.f)));

    // Forward that points down
    // mObject->set_forward(glm::rotate(mObject->forward(), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f)));


    mObject->animated_mesh()->set_default_animation(1);
    // mObject->set_position(glm::vec3(0.f, 0.f, 0.f));

    prevMousePosition = engine::input::mouse_position();
}

void Player::onUpdate(const engine::timestep& timestep) {

    // idle animation
    // mObject->animated_mesh()->switch_animation(1);

    if (hasStarted) {
        if (mTransitionCameraTimer > 0.f) {
            mTransitionCameraTimer -= (float)timestep;
            if (mTransitionCameraTimer < 0.f) {
                canTransition = true;
            }
        }

        if (engine::input::key_pressed(engine::key_codes::KEY_T)) {
            if (canTransition) {
                mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));

                canTransition = false;
                mTransitionCameraTimer = 0.5f;
                firstPerson = !firstPerson;
            }
        }


        // timer for jumping
        if (mJumpTimer > 0.f) {
            mJumpTimer -= (float)timestep;
            if (mJumpTimer < 0.f) {
                mObject->animated_mesh()->switch_root_movement(false);
                mObject->animated_mesh()->switch_animation(mObject->animated_mesh()->default_animation());
                mSpeed = 1.0f;
            }
        }

        //turning
        currentMousePosition = engine::input::mouse_position();
        if (prevMousePosition.first > currentMousePosition.first) {
            turn(1.f * timestep);
        }
        // Turn Right if mouse moves right
        else if (prevMousePosition.first < currentMousePosition.first) {
            turn(-1.f * timestep);
        }
        mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));

        if (prevMousePosition.second>currentMousePosition.second) {
            camLookAtY += ySensitivity*timestep;
        }else if(prevMousePosition.second<currentMousePosition.second) {
            camLookAtY -= ySensitivity*timestep;
        }


        //
        // if (!firstPerson) {
        //     // const std::pair<float, float> currentMousePosition = engine::input::mouse_position();
        //     // Turn Left if mouse moves Left
        //     if (prevMousePosition.first > currentMousePosition.first) {
        //         turn(1.f * timeStep);
        //     }
        //     // Turn Right if mouse moves right
        //     else if (prevMousePosition.first < currentMousePosition.first) {
        //         turn(-1.f * timeStep);
        //     }
        //     mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
        //
        // }
        // else {
        //     // if in 1st person, then set the forward of the player to the forward of the camera
        //     // mObject->set_forward(glm::normalize(glm::vec3(cameraFront.x, cameraFront.y, cameraFront.z)));
        //
        //
        //     if (prevMousePosition.first > currentMousePosition.first) {
        //         turn(1.f * timeStep);
        //     }
        //     // Turn Right if mouse moves right
        //     else if (prevMousePosition.first < currentMousePosition.first) {
        //         turn(-1.f * timeStep);
        //     }
        //     mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
        //
        //
        // }

        // Get Strafing Directions
        const glm::vec3 up = glm::vec3(0, 1, 0);
        const glm::vec3 right = glm::cross(glm::normalize(mObject->forward()), glm::normalize(up));
        const glm::vec3 left = -right;

        // KEYBOARD CONTROLS
        // Press A to strafe Left
        if (engine::input::key_pressed(engine::key_codes::KEY_A)) {
            mObject->set_position(mObject->position() += left * mSpeed * static_cast<float>(timestep));
        }
            // Press D to strafe Right
        else if (engine::input::key_pressed(engine::key_codes::KEY_D)) {
            mObject->set_position(mObject->position() += right * mSpeed * static_cast<float>(timestep));
        }

        // Move Forward if W is pressed
        if (engine::input::key_pressed(engine::key_codes::KEY_W)) {
            mObject->animated_mesh()->default_animation();
            mObject->set_position(mObject->position() += mObject->forward() * mSpeed * static_cast<float>(timestep));
            mObject->set_position(glm::vec3(mObject->position().x, height / 2, mObject->position().z));
            mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
        }
        // Move Back if S is pressed
        else if (engine::input::key_pressed(engine::key_codes::KEY_S)) {
            mObject->set_position(mObject->position() -= mObject->forward() * mSpeed * (float)timestep);
            mObject->set_position(glm::vec3(mObject->position().x, height / 2, mObject->position().z));
            mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
        }

        // Jump if Space is pressed
        if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
            jump();
        }

        // Animate the mesh
        mObject->animated_mesh()->on_update(timestep);
    }
    else {
    }
}

void Player::turn(float angle) const {
    mObject->set_forward(glm::rotate(mObject->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void Player::jump() {
    mJumpTimer = mObject->animated_mesh()->animations().at(3)->mDuration;
    mObject->animated_mesh()->switch_root_movement(true);
    mObject->animated_mesh()->switch_animation(3);
    // mSpeed = 0.0f;
}

void Player::setHasStarted(const bool pHasStarted) {
    hasStarted = pHasStarted;
}

void Player::updateCamera3rdPerson(engine::perspective_camera& camera, const engine::timestep& timeStep) {
    float A = 2.f;
    float B = 3.f;
    float C = 6.f;

    glm::vec3 camPos = mObject->position() - glm::normalize(mObject->forward()) * B;
    if(!(camPos.y>=cameraPosY) && !(cameraPosY<0)) {
        camPos.y += A - camLookAtY;
    }
         

    glm::vec3 camLookAt = mObject->position() + glm::normalize(mObject->forward()) * C;
    camLookAt.y = camLookAtY;
    // camera.set_view_matrix_custom(cam_pos, cam_look_at, mObject->position(), timeStep);
    camera.set_view_matrix(camPos, camLookAt);
}


void Player::update1stPersonCamera(engine::perspective_camera& camera, const engine::timestep& timestep) {
    // auto cameraPosition = mObject->position();
    // cameraPosition.y += height;
    // cameraPosition.z -= 0;
    // camera.onUpdate1stPerson(timestep, cameraPosition);
    float A = height;
    float B = -0.1f;
    float C = 6.f;

    glm::vec3 cam_pos = mObject->position() - glm::normalize(mObject->forward()) * B;

    cam_pos.y += A;

    glm::vec3 cam_look_at = mObject->position() + glm::normalize(mObject->forward()) * C;
    cam_look_at.y -= camLookAtY;
    // camera.set_view_matrix_custom(cam_pos, cam_look_at, mObject->position(), timeStep);
    camera.set_view_matrix(cam_pos, cam_look_at);
}

void Player::updateCamera(engine::perspective_camera& camera, const engine::timestep& timestep) {
    cameraFront = camera.front_vector();

    if (firstPerson) {
        update1stPersonCamera(camera, timestep);
    }
    else {
        updateCamera3rdPerson(camera, timestep);
    }
}
