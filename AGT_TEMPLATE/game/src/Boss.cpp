#include "Boss.h"

Boss::Boss() {}
Boss::~Boss() {}

void Boss::initialise() {

    // create the boss game object
    //
    // model from https://www.turbosquid.com/3d-models/3d-humanoid-robot-character-1479200
    engine::ref<engine::model> bossModel = engine::model::create("assets/models/static/Argon.obj");
    engine::game_object_properties bossProps;
    bossProps.meshes = bossModel->meshes();
    bossProps.scale = glm::vec3(
        2.5f / glm::max(bossModel->size().x, glm::max(bossModel->size().y, bossModel->size().z)));
    bossProps.bounding_shape = glm::vec3(
        2.5f / glm::max(bossModel->size().x, glm::max(bossModel->size().y, bossModel->size().z)));
    bossProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    bossProps.type = 0;
    // https://opengameart.org/node/21067
    bossProps.textures = {engine::texture_2d::create("assets/textures/antimony.png", false)};
    mObject = engine::game_object::create(bossProps);
    // set the forward and position for the boss
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, -10.f));


    // create the bullet shape
    const engine::ref<engine::BulletShape> bulletShape = engine::BulletShape::createDefaultVertices(0.25f, 0.1f, 0.25f);
    const std::vector<engine::ref<engine::texture_2d>> bulletTextures = {
        // https://opengameart.org/content/details-for-damaged-and-dirty-textures
        engine::texture_2d::create("assets/textures/bullet.png", false)
    };
    engine::game_object_properties bulletProps;
    bulletProps.position = {mObject->position().x, mObject->position().y + 1.f, mObject->position().z};
    bulletProps.meshes = {bulletShape->mesh()};
    bulletProps.textures = bulletTextures;
    auto bulletObject = engine::game_object::create(bulletProps);
    mBullet = bulletObject;
}

// render the boss and bullet
void Boss::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader,
                    const engine::perspective_camera& camera) const {

    // only render the bullet if show bullet is true
    if (showBullet) {
        engine::renderer::submit(texturedLightingShader, mBullet);
    }

    // only render the boss if it is not dead
    if (!isDead)
        engine::renderer::submit(texturedLightingShader, mObject);
}

// render the 2d elements of the boss (the boss health)
void Boss::onRenderHUD(engine::ref<engine::text_manager>& textManager) {
    // render the health of the boss if it is not in an idle state
    if (mState != State::IDLE) {
        const auto healthText = "BOSS: " + std::to_string(mHealthPoints);
        const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
        textManager->render_text(text_shader, healthText, 500.f, (float)engine::application::window().height() - 50.f, 1.f, glm::vec4(1.f, 0.f, 0.1f, 1.f));
    }
}


void Boss::onUpdate(const engine::timestep& timestep, Player& player, BillboardManager& billboardManager) {
    if (!isDead) {
        // turn towards the player
        const glm::vec3 v = player.object()->position() - mObject->position();
        const float theta = atan2(v.x, v.z);
        turn(theta);

        // update the position of the explosion billboard to the position of the boss
        billboardManager.getSfx(BillboardManager::sfx_EXPLOSION).position = glm::vec3(
            mObject->position().x, mObject->position().y + 0.5f, mObject->position().z + 0.1f);

        // calculate the distance of the boss to the player, used in the many switch statements
        const auto distanceFromPlayer = glm::length(mObject->position() - player.object()->position());


        // Switch Case for the base state of the boss
        switch (mState) {
        case State::IDLE: // idle non-aggro state
            // if the distance is less than 3.f, the boss will activate and start by moving.
            if (distanceFromPlayer < 3.f) {
                mState = State::MOVE;
            }
            break;
        case State::MOVE: // move state

            // pick the movement 50:50 chance
            if (randomNumber() < 50) {
                mMoveState = MoveState::STRAFE;
            }
            else {
                mMoveState = MoveState::DASH;
            }

            // Switch case for the move state
            switch (mMoveState) {
            case MoveState::STRAFE:
                // activate strafing
                strafeTimer = 0.5f;
                // 50:50 chance on whether to strafe right or left
                if (randomNumber() < 50) {
                    direction = false;
                }
                else {
                    direction = true;
                }
                // tell the system it is in an acting state
                mState = State::ACTING;
                break;
            case MoveState::DASH:
                // activate dashing
                dashTimer = 0.5f;

                // 50:50 chance on whether to dash forwards or backwards
                if (randomNumber() < 50) {
                    direction = false;
                }
                else {
                    direction = true;
                }
                // tell the system it is in an acting state
                mState = State::ACTING;
                break;
            default: ;
            }
            break;
        case State::ATTACK: // attack state
            mState = State::OPEN;
            // CODE COMMENTED OUT FOR THE POTENTIAL AIR ATTACK STATE
            // if (randomNo < 20) {
            //     //air attack
            //     mAttackState = AttackState::AIR_ATTACK;
            // }
            // else if (randomNo < 60) {
            //     //fast bullet
            //     mAttackState = AttackState::FAST_SHOT;
            // }
            // else {
            //     // track bullet
            //     mAttackState = AttackState::TRACKING_SHOT;
            // }
            //

            // Pick the attack to perform, 60:40 chance of fast shot to tracking shot
            if (randomNumber() < 60) {
                mAttackState = AttackState::FAST_SHOT;
            }
            else {
                mAttackState = AttackState::TRACKING_SHOT;
            }

            switch (mAttackState) {
            case AttackState::TRACKING_SHOT:

                // set the scale, speed and tracking of the bullet
                mBullet->set_scale(glm::vec3(3.f));
                bulletSpeed = 2.f;
                isTracking = true;
                bulletCollisionRadius = 2.5f;

                // shoot the bullet
                if (bulletTimer <= 0) {
                    bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
                    shoot(billboardManager);
                }
                // change the state to the acting state
                mState = State::ACTING;
                break;
            case AttackState::FAST_SHOT:

                // set the scale, speed and tracking of the bullet
                mBullet->set_scale(glm::vec3(1.f));
                bulletSpeed = 4.f;
                isTracking = false;
                bulletCollisionRadius = 1.5f;

                // shoot the bullet
                if (bulletTimer <= 0) {
                    bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
                    shoot(billboardManager);
                }
                // change the state to the acting state
                mState = State::ACTING;

                break;
            case AttackState::AIR_ATTACK:
                // not implemented, not enough time
                // std::cout << "Air Attack \n";
                break;
            default:
                break;
            }

            break;
        case State::OPEN: // Open/Vulnerable state
            // tick down the timer
            if (openTimer > 0) {
                openTimer -= static_cast<float>(timestep);
            }

            // if the timer has finished choose the state to go into next
            if (openTimer <= 0) {
                // pick state here using random numbers 45% chance for attack and 55% chance for move
                // picking here as just coming out of the open state
                if (randomNumber() < 45) {
                    mState = State::ATTACK;
                }
                else {
                    mState = State::MOVE;
                }
            }

            break;

        case State::ACTING: // a state to complete the actions that need to be performed

            // after each action the boss is put into the open state where it will be vulnerable for a bit and then choose the next action

            // if the action was to strafe, tick down the strafe timer and call the strafe method
            if (strafeTimer > 0) {
                strafe(timestep);
                strafeTimer -= timestep;
                if (strafeTimer <= 0) {
                    openTimer = 3.f;
                    mState = State::OPEN;
                }
            }

            // if the action was to strafe, tick down the strafe timer and call the strafe method
            if (dashTimer > 0) {
                dash(timestep);
                dashTimer -= static_cast<float>(timestep);
                if (dashTimer <= 0) {
                    openTimer = 3.f;
                    mState = State::OPEN;
                }
            }

            // Handling the timer for the bullet shooting
            if (bulletTimer > 0.f) {
                if (isTracking)
                    bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
                mBullet->set_position(mBullet->position() += bulletTrajectory * glm::vec3(timestep) * bulletSpeed);
                //check collision with the player
                checkPlayerBulletCollision(player);
                bulletTimer -= static_cast<float>(timestep);
                if (bulletTimer <= 0.f) {
                    showBullet = false;
                    mBullet->set_position(glm::vec3(mObject->position().x, mObject->position().y + 1.f, mObject->position().z));
                    openTimer = 3.f;
                    mState = State::OPEN;

                    // planned to do another billboard explosion
                }
            }


            break;
        default:
            break;
        }


        // handling timer for getting hit
        // on a timer as the collision for the sword can be colliding for a bit and it will destroy boss to quickly
        if (getHitTimer > 0.f) {
            getHitTimer -= static_cast<float>(timestep);
        }

        // a timer to manage the collision with the player, will only hit the player every half a second so that the player's
        // health will not be depleted instantly if he stays in the collision.
        if (bulletHitTimer > 0) {
            bulletHitTimer -= timestep;
        }

    }
    else {
        // if dead hide the bullet
        showBullet = false;

    }
}

// check the collision with the player and the bullet
void Boss::checkPlayerBulletCollision(Player& player) {
    const auto distance = glm::length(mBullet->position() - player.object()->position());
    if (distance <= bulletCollisionRadius) {
        if (bulletHitTimer <= 0) {
            player.getHit(10 * bulletCollisionRadius); // damage is based on the size of the bullet
            bulletHitTimer = 1.f;
        }
    }
}

// dash forwards or backwards
void Boss::dash(const engine::timestep& timestep) {
    auto offset = 2.3f * static_cast<float>(timestep);
    if (direction)
        offset = offset * -1.f;
    mObject->set_position(glm::vec3(mObject->position().x, mObject->position().y, mObject->position().z + offset));
}


// strafe left or right
void Boss::strafe(const engine::timestep& timestep) {
    auto offset = 1.5f * static_cast<float>(timestep);
    if (direction)
        offset = offset * -1.f;
    mObject->set_position(glm::vec3(mObject->position().x + offset, mObject->position().y, mObject->position().z));
}

// deduct damage from the health points if hit
void Boss::getHit(const int& damage) {
    if (getHitTimer <= 0.f) {
        mHealthPoints -= damage;
        if (mHealthPoints <= 0) {
            mHealthPoints = 0;
            die();
        }
        getHitTimer = 1.f;
    }
}

// Turn Boss and the bullet towards the player
void Boss::turn(float angle) const {
    if (mState != State::IDLE) {
        mObject->set_rotation_amount(angle);
        if (bulletTimer <= 0 || isTracking)
            mBullet->set_rotation_amount(angle);
    }
}

// shoot a bullet and do an explosion
void Boss::shoot(BillboardManager& billboardManager) {
    billboardManager.activate(billboardManager.getSfx(BillboardManager::sfx_EXPLOSION));
    showBullet = true;
    bulletTimer = 2.5f;
}

// set the is dead variable to true. can be used to create a unique billboard effect on death
void Boss::die() {
    isDead = true;
}

// generate a random number, used in choosing what states to switch to and what directions to pick
int Boss::randomNumber() {
    return rand() % 100 + 1;
}
