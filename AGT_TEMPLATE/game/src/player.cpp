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
    mObject->animated_mesh()->set_default_animation(1);





    // mObject->set_position(glm::vec3(0.f, 0.f, 0.f));


    prevMousePosition = engine::input::mouse_position();
}

void Player::onUpdate(const engine::timestep& timeStep) {

    // idle animation
    // mObject->animated_mesh()->switch_animation(1);

    // timer for jumping
    if (mJumpTimer > 0.f) {
        mJumpTimer -= (float)timeStep;
        if (mJumpTimer < 0.f) {
            mObject->animated_mesh()->switch_root_movement(false);
            mObject->animated_mesh()->switch_animation(mObject->animated_mesh()->default_animation());
            mSpeed = 1.0f;
        }
    }

    std::pair<float, float> currentMousePosition = engine::input::mouse_position();
    if (prevMousePosition.first > currentMousePosition.first) {
        //Left
        turn(1.f * timeStep);
    }
    else if (prevMousePosition.first < currentMousePosition.first) {
        //right
        turn(-1.f * timeStep);
    }

    if (prevMousePosition.second > currentMousePosition.second) {
        
    }


    // KEYBOARD CONTROLS
    if (engine::input::key_pressed(engine::key_codes::KEY_A)) {
        turn(1.f * timeStep);
    }
    else if (engine::input::key_pressed(engine::key_codes::KEY_D)) {
        turn(-1.f * timeStep);
    }


    if (engine::input::key_pressed(engine::key_codes::KEY_W)) {
        mObject->animated_mesh()->default_animation();
        mObject->set_position(mObject->position() += mObject->forward() * mSpeed * (float)timeStep);
        mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
    }
    else if (engine::input::key_pressed(engine::key_codes::KEY_S)) {
        mObject->set_position(mObject->position() -= mObject->forward() * mSpeed * (float)timeStep);
        mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
    }

    if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
        jump();
    }

    mObject->animated_mesh()->on_update(timeStep);

    mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
}

void Player::turn(float angle) {
    mObject->set_forward(glm::rotate(mObject->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void Player::jump() {

    mJumpTimer = mObject->animated_mesh()->animations().at(3)->mDuration;
    mObject->animated_mesh()->switch_root_movement(true);
    mObject->animated_mesh()->switch_animation(3);
    // mSpeed = 0.0f;
}


void Player::updateCamera3rdPerson(engine::perspective_camera& camera, const engine::timestep& timeStep) {
    float A = 2.f;
    float B = 3.f;
    float C = 9.f;

    // std::pair<float, float> currentMousePosition = engine::input::mouse_position();

    // if(currentMousePosition.second>prevMousePosition.second) {
    //     C += 0.5f;
    // }else if(currentMousePosition.second<prevMousePosition.second) {
    //     C -= 0.5f;
    // }

    glm::vec3 cam_pos = mObject->position() - glm::normalize(mObject->forward()) * B;
    cam_pos.y += A;

    glm::vec3 cam_look_at = mObject->position() + glm::normalize(mObject->forward()) * C;
    cam_look_at.y = 0.f;
    // camera.set_view_matrix_custom(cam_pos, cam_look_at, mObject->position(), timeStep);
    camera.set_view_matrix(cam_pos, cam_look_at);










}


void Player::update1stPersonCamera(engine::perspective_camera& camera, const engine::timestep& timestep) {
    auto cameraPosition = mObject->position();
    cameraPosition.y += height;
    cameraPosition.z -= 0.1f;
    camera.onUpdate1stPerson(timestep, cameraPosition);

}

//https://www.youtube.com/watch?v=PoxDDZmctnU
void Player::updateCamera(engine::perspective_camera& camera) {
    // float sens = 0.01f;
    //
    // auto mousePosition = engine::input::mouse_position();
    //
    // // calculate pitch
    // float pitchChange = mousePosition.second * sens;
    // camera.getPitch() += pitchChange;
    //
    // //calculate angle around the player
    // float angleChange = mousePosition.first * sens;
    // camera.getAngleAroundPlayer() -= angleChange;
    //
    // //calculate horizontal distance
    // float horizontalDistance = cameraRadius * cos(glm::radians(camera.getPitch()));
    //
    // //calculate vertical distance
    // float verticalDistance = cameraRadius * sin(camera.getPitch());
    //
    //
    // // calculate camera position
    // float theta = mObject->rotation_axis().y + camera.getAngleAroundPlayer();
    // float offsetX = horizontalDistance * sin(theta);
    // float offsetZ = horizontalDistance * cos(theta);
    //
    //
    // glm::vec3 cameraPosition = glm::vec3(mObject->position().x - offsetX, camera.position().y + verticalDistance,
    //                                      mObject->position().z - offsetZ);
    // camera.position(cameraPosition);
    //
    // camera.getYaw() = (atan(1) * 4) - (mObject->rotation_axis().y + camera.getAngleAroundPlayer());
    //
    // // glm::lookAt()

}
