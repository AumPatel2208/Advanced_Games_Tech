#pragma once
#include <engine.h>
#include "player.h"
#include "BillboardManager.h"

class Boss {

    enum class State {
        IDLE,
        MOVE,
        ATTACK,
        OPEN
    };

    enum class AttackState {
        TRACKING_SHOT,
        FAST_SHOT,
        AIR_ATTACK
    };

    enum class MoveState {
        STRAFE,
        DASH
    };

public:
    Boss();
    ~Boss();

    // Initialise the NPC
    void initialise();

    void onRender(const std::shared_ptr<engine::shader>& texturedLightingShader, const engine::perspective_camera& camera) const;
    void moveBullet();
    void onUpdate(const engine::timestep& timestep, Player& player, BillboardManager& billboardManager);
    void turn(float angle) const;
    void shoot(BillboardManager& billboardManager);

    //getters/setters
    engine::ref<engine::game_object> object() { return mObject; }


private:
    engine::ref<engine::game_object> mObject;
    engine::ref<engine::audio_manager> mAudioManager{};
    engine::ref<engine::game_object> mBullet;
    glm::vec3 bulletTrajectory{};
    bool showBullet = false;

    State mState = State::IDLE;
    MoveState mMoveState = MoveState::STRAFE;
    AttackState mAttackState = AttackState::TRACKING_SHOT;

    float bulletTimer = 0.f;
    
};
