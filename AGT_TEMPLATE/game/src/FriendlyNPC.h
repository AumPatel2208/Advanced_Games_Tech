#pragma once
#include <engine.h>
#include "AnimationHandler.h"
#include "player.h"

class FriendlyNPC {

public:
    FriendlyNPC();
    ~FriendlyNPC();

    // Initialise the NPC
    void initialise();
    void talk();

    void onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const;
    void renderChoiceHUD(engine::ref<engine::text_manager>& textManager);
    void onUpdate(const engine::timestep& timestep, Player& player);
    
    void playDialogue() const;


    float dialogueTimer1() const { return mDialogueTimer; }
    void setMDialogueTimer(float mDialogueTimer) { this->mDialogueTimer = mDialogueTimer; }
    void progress();

    //getters/setters
    engine::ref<engine::game_object> object() { return mObject; }
    // void setMObject(const engine::ref<engine::game_object>& mObject) { this->mObject = mObject; }

    void setIsBossBeaten(bool isBossBeaten) { this->isBossBeaten = isBossBeaten; }

private:
    engine::ref<engine::game_object> mObject;
    engine::ref<engine::audio_manager> mAudioManager {};

    bool isYes = false;
    bool isNo = false;
    bool isBossBeaten = false;
    bool toPLayDialogue = false;
    bool inChoice = false;
    bool toRenderChoiceHUD = false;
    bool isInRange = false;
    
    

    float mDialogueTimer = 0.f;
    std::string dialoguePath = "greeting";
    int         dialogueNumber = 1;

};
