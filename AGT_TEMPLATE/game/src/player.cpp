#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


Player::Player() {
    mJumpTimer = 0.f;
    mMovementTimer = 0.f;
    mStaminaRecoveryTimer = 0.f;
    mSpeed = 1.f;
}


Player::~Player() {}


// initialise the players object.
void Player::initialise() {
    // create skinned mesh for the player object applied to the player
    engine::ref<engine::skinned_mesh> mSkinnedMesh = engine::skinned_mesh::create(
        "assets/models/animated/mannequin/free3Dmodel.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/walk.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
    mSkinnedMesh->switch_root_movement(false);

    engine::game_object_properties objectProperties;
    objectProperties.animated_mesh = mSkinnedMesh;
    objectProperties.scale = glm::vec3(1.f / glm::max(mSkinnedMesh->size().x,
                                                      glm::max(mSkinnedMesh->size().y, mSkinnedMesh->size().z)));
    objectProperties.position = glm::vec3(3.0f, 0.5f, -5.0f);
    objectProperties.type = 0;
    objectProperties.bounding_shape = mSkinnedMesh->size() / 2.f * objectProperties.scale.x;

    mObject = engine::game_object::create(objectProperties);


    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));

    //Set the animations based on the objeect loaded
    // animationHandler = AnimationHandler(mObject);
    animationHandler = AnimationHandler(mObject);
    animationHandler.setAnimWalk(0);
    animationHandler.setIdleAnim(1);
    animationHandler.setAnimJump(2);
    animationHandler.setAnimRun(3);
    std::cout << "Anim Walk" << animationHandler.animWalk() << "\n";
    std::cout << "Anim Idle" << animationHandler.animIdle() << "\n";
    animationHandler.nextAnimation(animationHandler.animIdle());

    prevMousePosition = engine::input::mouse_position();

    //Bullet shape
    const engine::ref<engine::BulletShape> swordShape = engine::BulletShape::createDefaultVertices(
        -0.30f, 0.125f, 0.05f);
    const std::vector<engine::ref<engine::texture_2d>> swordTextures = {
        // reference https://www.filterforge.com/filters/4486-diffuse.html
        engine::texture_2d::create("assets/textures/sword_texture.jpg", false)
    };
    engine::game_object_properties swordProps;
    // swordProps.position = { mObject->position().x + swordOffsetX, mObject->position().y + swordOffsetY, mObject->position().z + swordOffsetZ };
    swordProps.position = {mObject->position().x, mObject->position().y, mObject->position().z};
    swordProps.meshes = {swordShape->mesh()};
    swordProps.textures = swordTextures;
    auto swordObject = engine::game_object::create(swordProps);
    mSword = swordObject;

    m_cross_fade = cross_fade::create("assets/textures/Red.bmp", 2.0f, 1.6f, 0.9f);

    // assign the current mouse position as previous as it will be empty otherwise
}

// temporary method to move the player into position for the first level model
void Player::moveIntoLevel1() const {
    mObject->set_position(glm::vec3(0.16f, mObject->position().y, 1.72f));
    mObject->set_forward(glm::vec3(0.14f, 0, 1.f));
}

void Player::onRenderStaticItems(const std::shared_ptr<engine::shader>& texturedLightingShader) const {
    engine::renderer::submit(texturedLightingShader, mSword);
}

void Player::render2d(const std::shared_ptr<engine::shader>& texturedLightingShader, engine::orthographic_camera camera) const {
    engine::renderer::begin_scene(camera, texturedLightingShader);
    m_cross_fade->on_render(texturedLightingShader);
    engine::renderer::end_scene();
}

// call every game loop
void Player::onUpdate(const engine::timestep& timestep) {
    m_cross_fade->on_update(timestep);
    if (crossFadeTimer > 0)
        crossFadeTimer -= timestep;

    if (hasStarted) {
        if (mTransitionCameraTimer > 0.f) {
            mTransitionCameraTimer -= static_cast<float>(timestep);
            if (mTransitionCameraTimer < 0.f) {
                canTransition = true;
            }
        }

        // change the camera mode (1st and 3rd person)
        if (engine::input::key_pressed(engine::key_codes::KEY_T)) {
            if (canTransition) {
                canTransition = false;
                // turn false to prevent transitioning. only for a short while as is changed to true after timer runs out
                mTransitionCameraTimer = 0.5f; // Timer to stop unwanted multiple key registrations
                firstPerson = !firstPerson; // flip the boolean
            }
        }


        // if (engine::input::key_pressed(engine::key_codes::KEY_E) && mDialogueTimer == 0.f) {
        //     toInteractWithNpc = true;
        // }


        // timer for jumping
        if (mJumpTimer > 0.f) {
            mJumpTimer -= static_cast<float>(timestep);
            if (mJumpTimer < 0.f) {
                mObject->animated_mesh()->switch_root_movement(false);
                // mObject->animated_mesh()->switch_animation(mObject->animated_mesh()->default_animation());
                animationHandler.nextAnimation(animationHandler.animIdle());
                // switch back to walking animation
                mSpeed = 1.0f; //reset speed back to 1
            }
        }

        //turning
        currentMousePosition = engine::input::mouse_position();

        if (prevMousePosition.first > currentMousePosition.first) {
            turn(1.f * timestep); // Turn Left if the mouse moves left
            // mSword->set_rotation_amount(mSword->rotation_amount() * timestep);
        }
        else if (prevMousePosition.first < currentMousePosition.first) {
            turn(-1.f * timestep); // Turn Right if mouse moves right
            // mSword->set_rotation_amount(mSword->rotation_amount() * -timestep);
        }


        // Set the rotation to the new forward
        mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));

        // y Sensitivity of the mouse/camera y Sensitivity of the mouse/camera 
        float ySens; // change based on if it is 1st person or 3rd
        if (firstPerson)
            ySens = ySensitivity1stPerson;
        else
            ySens = ySensitivity3rdPerson;
        if (prevMousePosition.second > currentMousePosition.second) {
            camLookAtY += ySens * timestep;
        }
        else if (prevMousePosition.second < currentMousePosition.second) {
            camLookAtY -= ySens * timestep;
        }

        // Get Strafing Directions
        const glm::vec3 up = glm::vec3(0, 1, 0);
        const glm::vec3 right = glm::cross(glm::normalize(mObject->forward()), glm::normalize(up));
        // do a cross product with the forward vector and up vector to return the 'right' direction vector
        const glm::vec3 left = -right; // left direction vector is the negative of the right

        // KEYBOARD CONTROLS
        // Press A to strafe Left
        if (engine::input::key_pressed(engine::key_codes::KEY_A)) {
            mObject->set_position(mObject->position() += left * mSpeed * static_cast<float>(timestep));
        }
            // Press D to strafe Right
        else if (engine::input::key_pressed(engine::key_codes::KEY_D)) {
            mObject->set_position(mObject->position() += right * mSpeed * static_cast<float>(timestep));
        }


        // timer for movement
        if (mMovementTimer > 0.f) {
            mMovementTimer -= static_cast<float>(timestep);
            if (mMovementTimer < 0.f) {
                // mObject->animated_mesh()->switch_root_movement(false);
                // animationHandler.nextAnimation(animationHandler.animIdle());
                idle(timestep);
                // switch back to walking animation
                mSpeed = 1.0f; //reset speed back to 1
            }
        }

            // Move Forward if W is pressed
            if (engine::input::key_pressed(engine::key_codes::KEY_W)) {
                // mObject->animated_mesh()->switch_animation(0);
                walk(true, timestep);
                mMovementTimer = 1.f;
            }
            // Move Back if S is pressed
            else if (engine::input::key_pressed(engine::key_codes::KEY_S)) {
                // mObject->animated_mesh()->switch_animation(0);
                walk(false, timestep);
                mMovementTimer = 1.f;
            }

            // Jump if Space is pressed
            if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
                jump();
                mMovementTimer = 1.f;
            }


            // reduce stamina on mouse click
            if (engine::input::mouse_button_pressed(0) && mStamina > 10.f) {
                if (!isSwordSwinging) {
                    // mStamina -= 10.f;
                    // mStaminaRecoveryTimer = 1.f;
                    decreaseStamina(10);
                    isSwordSwinging = true;
                    mSwordSwingTimer = 0.5f;
                }
            }

        if (mStaminaRecoveryTimer <= 0.f && mStamina < 100.f) {
            mStamina += mStaminaRecoverySpeed * timestep;
        }

        if (mStaminaRecoveryTimer > 0.f) {
            mStaminaRecoveryTimer -= timestep;
        }

        // Animate the mesh
        animationHandler.onUpdate(timestep);
        // mObject->animated_mesh()->on_update(timestep);

        // Move the sword
        //turn it then offset it
        // const glm::vec3 v = player.object()->position() - mObject->position();
        // const float theta = atan2(v.x, v.z);

        if (mSwordSwingTimer > 0) {
            swingSword(timestep);
            mSwordSwingTimer -= timestep;
            if (mSwordSwingTimer <= 0) {
                isSwordSwinging = false;
                swordOffsetX = 0.2f;
            }
        }


        if (pickupTimer > 0) {
            pickupTimer -= timestep;
            if (pickupTimer <= 0) {
                mStaminaRecoverySpeed = mDefaultStaminaRecoverySpeed;
            }
        }


        // turn the sword
        // // turn towards the player
        // const glm::vec3 v = mSword->position() - mObject->position();
        // const float theta = atan2(v.x, v.z);
        // mSword->set_rotation_amount(theta);

        mSword->set_position(glm::vec3(mObject->position().x + swordOffsetX, mObject->position().y + swordOffsetY,
                                       mObject->position().z + swordOffsetZ));
    }
}

void Player::swingSword(const engine::timestep& timestep) {
    swordOffsetX -= 1.f * timestep;
}

void Player::renderHud(engine::ref<engine::text_manager>& textManager) const {
    if (hasStarted) {
        if (isAlive) {
            // form the text
            const auto healthText = "Health: " + std::to_string(mHealthPoints);
            const auto staminaText = "Stamina: " + std::to_string(static_cast<int>(mStamina));
            const auto scoreText = "Score: " + std::to_string(mScore);

            // Render text
            const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
            textManager->render_text(text_shader, healthText, 10.f, 25.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
            textManager->render_text(text_shader, staminaText, 10.f, 100.f, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f));
            textManager->render_text(text_shader, scoreText, 10.f, (float)engine::application::window().height() - 50.f, 1.f, glm::vec4(1.f, 0.f, 0.5f, 1.f));

            if (pickupTimer > 0) {
                textManager->render_text(text_shader, "* stamina up", 10.f, 175.f, 0.5f, glm::vec4(0.f, 1.f, 0.f, 1.f));
            }
        }else {
            const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

            textManager->render_text(text_shader, "GAME OVER", (float)engine::application::window().width()/2-100.f, (float)engine::application::window().height()/2-75.f, 2.f, glm::vec4(1.f, 0.f, 0.1f, 1.f));

        }
    }
}

void Player::idle(const engine::timestep timestep) {
    animationHandler.nextAnimation(animationHandler.animIdle());
}

void Player::walk(const bool& forward, const engine::timestep& timestep) {

    animationHandler.nextAnimation(animationHandler.animWalk());

    if (forward)
        mObject->set_position(mObject->position() += mObject->forward() * mSpeed * static_cast<float>(timestep));
    else
        mObject->set_position(mObject->position() -= mObject->forward() * mSpeed * static_cast<float>(timestep));
    mObject->set_position(glm::vec3(mObject->position().x, height / 2, mObject->position().z));
    mObject->set_rotation_amount(atan2(mObject->forward().x, mObject->forward().z));

}

void Player::getHit(const int& damage) {
    mHealthPoints -= damage;
    if (crossFadeTimer <= 0) {
        m_cross_fade->activate();
        crossFadeTimer = 0.5f;
    }
    if(mHealthPoints<=0) {
        mHealthPoints = 0;
        isAlive = false;
    }
}

void Player::turn(float angle) const {
    mObject->set_forward(glm::rotate(mObject->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
    mObject->set_rotation_amount(angle);
    // mSword->set_rotation_amount(glm::radians(angle));
}

void Player::jump() {
    animationHandler.nextAnimation(animationHandler.animJump());
    // timer set for the jump
    mJumpTimer = mObject->animated_mesh()->animations().at(animationHandler.animJump())->mDuration;
}

void Player::setHasStarted(const bool _hasStarted) {
    hasStarted = _hasStarted;
}

void Player::decreaseStamina(const float& amount) {
    mStamina -= amount;
    mStaminaRecoveryTimer = 1.f;
}

void Player::staminaPickup() {
    mStamina += 50.f;
}

void Player::healthPickup() {
    mHealthPoints += 50;
}

void Player::staminaRecoveryPickup() {
    mStaminaRecoverySpeed = 30.f;
    pickupTimer = 10.f;
}

void Player::increaseScore(const int amount) {
    mScore += amount;
}

void Player::updateCamera3rdPerson(engine::perspective_camera& camera, const engine::timestep& timeStep) {
    float A = 2.f;
    float B = 3.f;
    float C = 6.f;

    glm::vec3 camPos = mObject->position() - glm::normalize(mObject->forward()) * B;

    // limiting the cameras position when mouse is moved too high
    if (camLookAtY > 0.7f)
        camLookAtY = 0.7f;
    else if (camLookAtY < -1.77f)
        camLookAtY = -1.77f;

    camPos.y += A + camLookAtY;
    // camPos.y +=  camLookAtY;
    //limit at 2.7 high 0.77 min
    // std::cout << "Y pos: " <<  camPos.y << "\n";

    glm::vec3 camLookAt = mObject->position() + glm::normalize(mObject->forward()) * C;
    camLookAt.y = -camLookAtY;
    // limit at -0.21 min 1.73 max
    // std::cout << "Y lok: " <<  camLookAt.y << "\n";

    // camera.set_view_matrix_custom(cam_pos, cam_look_at, mObject->position(), timeStep);
    camera.set_view_matrix(camPos, camLookAt);
}

void Player::update1stPersonCamera(engine::perspective_camera& camera, const engine::timestep& timestep) const {
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
