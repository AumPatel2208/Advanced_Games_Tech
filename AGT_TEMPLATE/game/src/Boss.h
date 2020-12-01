#pragma once
#include <engine.h>
#include "player.h"
#include "BillboardManager.h"

class Boss {
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

    float bulletTimer = 0.f;
    
};
