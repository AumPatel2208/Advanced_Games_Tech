#include "FriendlyNPC.h"

FriendlyNPC::FriendlyNPC() {
    mAudioManager = engine::audio_manager::instance();
    mAudioManager->init();
    // Load the dialogue
    mAudioManager->load_sound("", engine::sound_type::event, "greeting_1");
    mAudioManager->load_sound("", engine::sound_type::event, "greeting_2");
    mAudioManager->load_sound("", engine::sound_type::event, "greeting_3");
    mAudioManager->load_sound("", engine::sound_type::event, "yes_1");
    mAudioManager->load_sound("", engine::sound_type::event, "yes_2");
    mAudioManager->load_sound("", engine::sound_type::event, "yes_3");
    mAudioManager->load_sound("", engine::sound_type::event, "no_1");
    mAudioManager->load_sound("", engine::sound_type::event, "no_2");
    mAudioManager->load_sound("", engine::sound_type::event, "no_3");
    mAudioManager->load_sound("", engine::sound_type::event, "end_1");
    mAudioManager->load_sound("", engine::sound_type::event, "end_2");
}

void FriendlyNPC::initialise() {
    engine::game_object_properties npcProps;
    npcProps.meshes = ;
    npcProps.textures = { engine::texture_2d::create("assets/models/animated/minotaur/Minotaur_diffuse.tga", false) };
    npcProps.type = 0;
    mObject = engine::game_object::create(npcProps);
    mObject->set_scale(glm::vec3(3.f));

}


void FriendlyNPC::onUpdate(const engine::timestep& timestep) {
    if (toPLayDialogue)
        playDialogue();

}

void FriendlyNPC::playDialogue() const {
    const std::string dialogue = dialoguePath + "_" + std::to_string(dialogueNumber);
    mAudioManager->play(dialogue);
}

void FriendlyNPC::progress() {
    if(dialoguePath == "greeting") {
        if (dialogueNumber == 1 || dialogueNumber == 2)
            dialogueNumber += 1;
        else {
            dialogueNumber = 1;
            if (isYes)
                dialoguePath = "yes";
            else
                dialoguePath = "no";
        }
    }else if(dialoguePath=="yes" || dialoguePath=="no") {
        if(dialogueNumber == 1 || dialogueNumber == 2) {
            dialogueNumber += 1;
        }
        else if (isBossBeaten && dialogueNumber == 3) {
            dialoguePath = "end";
            dialogueNumber = 1;
        }
        
    }else if(dialoguePath=="end" && isBossBeaten) {
        if(dialogueNumber == 1) {
            dialogueNumber += 1;
        }else if(dialogueNumber == 2) {
            
        }
    }
}
