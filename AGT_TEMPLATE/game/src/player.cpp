#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"

Player::Player() {
    mTimer = 0.f;
    mSpeed = 1.f;

}

Player::~Player() {}

void Player::initialise(engine::ref<engine::game_object> object) {
    mObject = object;
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));
    mObject->animated_mesh()->set_default_animation(1);
}

void Player::onUpdate(const engine::timestep& timeStep) {
    mObject->set_position(mObject->position() += mObject->forward() * mSpeed * (float)timeStep);
    mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));
    mObject->animated_mesh()->on_update(timeStep);

    if (mTimer > 0.f) {
        mTimer -= (float)timeStep;
        if (mTimer < 0.f) {
            mObject->animated_mesh()->switch_root_movement(false);
            mObject->animated_mesh()->switch_animation(mObject->animated_mesh()->default_animation());
            mSpeed = 1.0f;
        }
    }


    // KEYBOARD CONTROLS
    if (engine::input::key_pressed(engine::key_codes::KEY_1)) {
        turn(1.f * timeStep);
    }
    else if (engine::input::key_pressed(engine::key_codes::KEY_2)) {
        turn(-1.f * timeStep);
    }
    else if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
        jump();
    }
}

void Player::turn(float angle) {
    mObject->set_forward(glm::rotate(mObject->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void Player::jump() {

    mTimer = mObject->animated_mesh()->animations().at(3)->mDuration;
    mObject->animated_mesh()->switch_root_movement(true);
    mObject->animated_mesh()->switch_animation(3);
    mSpeed = 0.0f;
}


void Player::updateCameraOld(engine::perspective_camera& camera, const engine::timestep& timeStep) {

    /*
    // camera.set_view_matrix(glm::vec3(mObject->position().x, mObject->position().y + 1.f, mObject->position().z - 1.f), mObject->position() + 4.f);

    // camera.on_update(timeStep);

    float A = 2.f;
    float B = 3.f;
    float C = 6.f;

    //temp variable make it a member variable after
    // half way into players height
    const auto halfPlayerHeight = .8f;
    const auto radius = 3.f;

    auto midWayPosition  = mObject->position();
    midWayPosition.y += halfPlayerHeight;

    const auto [mouseDeltaX, mouseDeltaY] =  engine::input::mouse_position();

    std::cout << mouseDeltaX << " " << mouseDeltaY;

    //
    glm::vec3 mousePosition = glm::vec3(mouseDeltaX, mouseDeltaY, 0.f);
    auto v = mousePosition - mObject->position();
    auto cameraDirection = atan2(v.x,v.y);
    
    glm::vec3 camPos = mObject->position() - glm::normalize(mObject->forward()) * radius;
    camPos.y += A;
    
    // glm::vec3 camLookAt = mObject->position() + glm::normalize(mObject->forward()) * radius;
    glm::vec3 camLookAt = midWayPosition + glm::vec3(cos(cameraDirection), sin(cameraDirection), 0.f);
    camLookAt.y = 0.f;

    //stuck to player cam
    camera.set_view_matrix(camPos,camLookAt );

    // camera.on3rdPersonUpdate(timeStep, camPos, camLookAt);

    */


}

//https://www.youtube.com/watch?v=PoxDDZmctnU
void Player::updateCamera(engine::perspective_camera& camera) {
    float sens = 0.01f;

    auto mousePosition = engine::input::mouse_position();

    // calculate pitch
    float pitchChange = mousePosition.second * sens;
    camera.getPitch() += pitchChange;

    //calculate angle around the player
    float angleChange = mousePosition.first * sens;
    camera.getAngleAroundPlayer() -= angleChange;

    //calculate horizontal distance
    float horizontalDistance = cameraRadius * cos(camera.getPitch());

    //calculate vertical distance
    float verticalDistance = cameraRadius * sin(camera.getPitch());


    // calculate camera position
    float theta = mObject->rotation_axis().y + camera.getAngleAroundPlayer();
    float offsetX = horizontalDistance * sin(theta);
    float offsetZ = horizontalDistance * cos(theta);


    glm::vec3 cameraPosition = glm::vec3(mObject->position().x - offsetX, camera.position().y + verticalDistance,
                                         mObject->position().z - offsetZ);
    camera.position(cameraPosition);

    camera.getYaw() = (atan(1)*4) - (mObject->rotation_axis().y + camera.getAngleAroundPlayer());



}
