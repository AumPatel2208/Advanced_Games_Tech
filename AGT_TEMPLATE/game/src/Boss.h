#pragma once
#include <engine.h>
#include "player.h"
#include "BillboardManager.h"

class Boss {

    enum class State {
        IDLE,
        MOVE,
        ATTACK,
        ACTING,
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
    void onRenderHUD(engine::ref<engine::text_manager>& textManager);
    void moveBullet();
    void onUpdate(const engine::timestep& timestep, Player& player, BillboardManager& billboardManager);
    void checkPlayerBulletCollision(Player& player);
    void strafe(const engine::timestep& timestep);
    void dash(const engine::timestep& timestep);
    void getHit(const int& damage);
    void turn(float angle) const;
    void shoot(BillboardManager& billboardManager);
    void die();

    static int randomNumber();

    //getters/setters
    engine::ref<engine::game_object> object() { return mObject; }
    bool getIsDead() const { return isDead; }


private:
    engine::ref<engine::game_object> mObject;
    engine::ref<engine::audio_manager> mAudioManager{};
    engine::ref<engine::game_object> mBullet;
    float bulletCollisionRadius = 1.f;
    float bulletSpeed = 2.f;
    float bulletHitTimer = 0.f;
    glm::vec3 bulletTrajectory{};
    bool showBullet = false;
    bool isDead = false;
    bool isTracking = false;
    bool direction = false; // false for left and backwards, true for right and forwards
    int mHealthPoints = 100;
    State mState = State::IDLE; 
    MoveState mMoveState = MoveState::STRAFE;
    AttackState mAttackState = AttackState::TRACKING_SHOT;

    float getHitTimer = 0.f;
    float bulletTimer = 0.f;

    
    float strafeTimer = 0.f;
    float dashTimer = 0.f;
    float openTimer = 0.f;
};
