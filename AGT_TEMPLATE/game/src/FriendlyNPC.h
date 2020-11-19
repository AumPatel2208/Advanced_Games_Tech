#pragma once
#include <engine.h>
#include "AnimationHandler.h"

class FriendlyNPC {
public:
    FriendlyNPC();
    ~FriendlyNPC();

    // Initialise the NPC
    void initialise();
    void talk();

    void onUpdate(const engine::timestep& timestep);

    void playDialogue() const;

    void progress();

private:
    engine::ref<engine::audio_manager> mAudioManager {};

    bool isYes = false;
    bool isBossBeaten = false;
    bool toPLayDialogue = false;
    
    std::string dialoguePath = "greeting";
    int         dialogueNumber = 1;

    engine::ref<engine::game_object> mObject;

};
