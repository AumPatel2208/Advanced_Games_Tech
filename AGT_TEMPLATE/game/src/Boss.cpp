#include "Boss.h"

Boss::Boss() {}
Boss::~Boss() {}

void Boss::initialise() {
    // model from https://www.turbosquid.com/3d-models/3d-humanoid-robot-character-1479200
    engine::ref<engine::model> bossModel = engine::model::create("assets/models/static/Argon.obj");
    engine::game_object_properties bossProps;
    // npcProps.animated_mesh =  npcMesh ;
    bossProps.meshes = bossModel->meshes();
    // npcProps.scale = glm::vec3(100);
    bossProps.scale = glm::vec3(
        2.5f / glm::max(bossModel->size().x, glm::max(bossModel->size().y, bossModel->size().z)));
    bossProps.bounding_shape = glm::vec3(
        2.5f / glm::max(bossModel->size().x, glm::max(bossModel->size().y, bossModel->size().z)));
    // npcProps.scale = glm::vec3(1.5);
    bossProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    // npcProps.bounding_shape = npcMesh->size() / 2.f * npcProps.scale.x;
    // npcProps.textures = { engine::texture_2d::create("assets/models/animated/minotaur/Minotaur_diffuse.tga", false) };
    bossProps.type = 0;
    // https://opengameart.org/node/21067
    bossProps.textures = {engine::texture_2d::create("assets/textures/antimony.png", false)};
    mObject = engine::game_object::create(bossProps);
    // mObject->set_scale(glm::vec3(3.f));
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, -10.f));
    // mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));
    // mObject->set_offset(bossModel->offset());
    // mObject->set_position(glm::vec3(mObject->position().x, mObject->position().y + 2.f, mObject->position().z));
    // mObject->set_mass(0);
    // mObject->set_velocity(glm::vec3(0.f, 9.8f, 5.f));

    //Bullet shape
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
    // mBullet->set_rotation_amount(glm::radians(90.f));
}

void Boss::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader,
                    const engine::perspective_camera& camera) const {

    if (showBullet) {
        engine::renderer::submit(texturedLightingShader, mBullet);
    }

    if (!isDead)
        engine::renderer::submit(texturedLightingShader, mObject);
}

void Boss::onRenderHUD(engine::ref<engine::text_manager>& textManager) {
    if (mState != State::IDLE) {
        const auto healthText = "BOSS: " + std::to_string(mHealthPoints);
        const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
        textManager->render_text(text_shader, healthText, 500.f, (float)engine::application::window().height() - 50.f, 1.f, glm::vec4(1.f, 0.f, 0.1f, 1.f));
    }
}

void Boss::moveBullet() { }

void Boss::onUpdate(const engine::timestep& timestep, Player& player, BillboardManager& billboardManager) {
    if (!isDead) {
        // turn towards the player
        const glm::vec3 v = player.object()->position() - mObject->position();
        const float theta = atan2(v.x, v.z);
        // activate turning
        turn(theta);


        billboardManager.getSfx(BillboardManager::sfx_EXPLOSION).position = glm::vec3(
            mObject->position().x, mObject->position().y + 0.5f, mObject->position().z + 0.1f);


        // if (engine::input::key_pressed(engine::key_codes::KEY_F)) {
        //     if (bulletTimer <= 0) {
        //         bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
        //         shoot(billboardManager);
        //     }
        // }

        // int randomNo = rand() % 100 + 1;

        const auto distanceFromPlayer = glm::length(mObject->position() - player.object()->position());

        // auto randomNo = randomNumber();


        switch (mState) {
        case State::IDLE: // idle non-aggro state
            if (distanceFromPlayer < 3.f) {
                mState = State::MOVE;
            }
            break;
        case State::MOVE: // move state

            // pick the movement
            if (randomNumber() < 50) {
                mMoveState = MoveState::STRAFE;
            }
            else {
                mMoveState = MoveState::DASH;
            }

            switch (mMoveState) {
            case MoveState::STRAFE:
                // activate strafing
                strafeTimer = 0.5f;
                // std::cout << "Strafing: " << randomNo << "\n";
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
                // std::cout << "Dash: " << randomNo << "\n";

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
            // std::cout << "Attack: ";
            mState = State::OPEN;
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
            if (randomNumber() < 60) {
                mAttackState = AttackState::FAST_SHOT;
            }
            else {
                mAttackState = AttackState::TRACKING_SHOT;
            }

            switch (mAttackState) {
            case AttackState::TRACKING_SHOT:
                // std::cout << "Tracking Shot \n";

                mBullet->set_scale(glm::vec3(3.f));
                bulletSpeed = 2.f;
                isTracking = true;
                bulletCollisionRadius = 2.5f;

                if (bulletTimer <= 0) {
                    bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
                    shoot(billboardManager);
                }
                mState = State::ACTING;
                break;
            case AttackState::FAST_SHOT:
                // std::cout << "Fast Shot \n";

                mBullet->set_scale(glm::vec3(1.f));
                bulletSpeed = 4.f;
                isTracking = false;
                bulletCollisionRadius = 1.5f;

                if (bulletTimer <= 0) {
                    bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
                    shoot(billboardManager);
                }
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
        case State::OPEN:
            if (openTimer > 0) {
                openTimer -= static_cast<float>(timestep);
            }
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

        case State::ACTING:
            if (strafeTimer > 0) {
                strafe(timestep);
                strafeTimer -= timestep;
                if (strafeTimer <= 0) {
                    openTimer = 3.f;
                    mState = State::OPEN;
                }
            }

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

                    // do another explosion
                }
            }


            break;
        default:
            break;
        }


        // handling timer for getting hit
        if (getHitTimer > 0.f) {
            getHitTimer -= static_cast<float>(timestep);
        }

        if(bulletHitTimer>0) {
            bulletHitTimer -= timestep;
        }

    }
    else {
        // if dead
        showBullet = false;

    }
}

void Boss::checkPlayerBulletCollision(Player& player) {
    const auto distance = glm::length(mBullet->position() - player.object()->position());
    if(distance<=bulletCollisionRadius) {
        if (bulletHitTimer <= 0) {
            player.getHit(10 * bulletCollisionRadius); // damage is based on the size of the bullet
            bulletHitTimer = 1.f;
        }
    }
}


void Boss::dash(const engine::timestep& timestep) {
    auto offset = 2.3f * static_cast<float>(timestep);
    if (direction)
        offset = offset * -1.f;
    mObject->set_position(glm::vec3(mObject->position().x, mObject->position().y, mObject->position().z + offset));
}


void Boss::strafe(const engine::timestep& timestep) {
    auto offset = 1.5f * static_cast<float>(timestep);
    if (direction)
        offset = offset * -1.f;
    mObject->set_position(glm::vec3(mObject->position().x + offset, mObject->position().y, mObject->position().z));
}


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

// Turn Boss towards the player
void Boss::turn(float angle) const {
    if (mState != State::IDLE) {
        mObject->set_rotation_amount(angle);
        if (bulletTimer <= 0 || isTracking)
            mBullet->set_rotation_amount(angle);
    }
}

void Boss::shoot(BillboardManager& billboardManager) {
    billboardManager.activate(billboardManager.getSfx(BillboardManager::sfx_EXPLOSION));
    showBullet = true;
    bulletTimer = 2.5f;
}

void Boss::die() {
    // do something
    isDead = true;
}

int Boss::randomNumber() {
    return rand() % 100 + 1;
}
