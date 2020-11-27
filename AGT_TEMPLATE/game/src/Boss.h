#pragma once
#include <engine.h>
#include "player.h"

class Boss {
public:
    Boss();
    ~Boss();

    // Initialise the NPC
    void initialise();

    void onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const;
    void onUpdate(const engine::timestep& timestep, Player& player);
    void turn(float angle) const;

    //getters/setters
    engine::ref<engine::game_object> object() { return mObject; }


private:
    engine::ref<engine::game_object> mObject;
    engine::ref<engine::audio_manager> mAudioManager {};
};

