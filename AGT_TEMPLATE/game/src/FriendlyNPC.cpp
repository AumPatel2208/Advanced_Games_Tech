#include "FriendlyNPC.h"

// #include "player.h"

FriendlyNPC::FriendlyNPC() {
    mAudioManager = engine::audio_manager::instance();

    // npcMesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
    // npcMesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
    // npcMesh->switch_root_movement(false);

    // npcMesh->set_default_animation(0);
}

FriendlyNPC::~FriendlyNPC() {}

void FriendlyNPC::initialise() {
    engine::ref<engine::model> npcModel = engine::model::create("assets/models/static/npc.dae");
    engine::game_object_properties npcProps;
    // npcProps.animated_mesh =  npcMesh ;
    npcProps.meshes = npcModel->meshes();
    // npcProps.scale = glm::vec3(100);
    npcProps.scale = glm::vec3(1.f / glm::max(npcModel->size().x, glm::max(npcModel->size().y, npcModel->size().z)));
    // npcProps.scale = glm::vec3(1.5);
    npcProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    // npcProps.bounding_shape = npcMesh->size() / 2.f * npcProps.scale.x;
    // npcProps.textures = { engine::texture_2d::create("assets/models/animated/minotaur/Minotaur_diffuse.tga", false) };
    npcProps.type = 0;
    mObject = engine::game_object::create(npcProps);
    // mObject->set_scale(glm::vec3(3.f));
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));
    // mObject->set_rotation_amount(glm::rotate(mObject.ro))

       // mObject->transform(npcTransform);


    // mObject->animated_mesh()->switch_animation(0);

    //Audio
    mAudioManager->init();
    // Load the dialogue
    mAudioManager->load_sound("assets/audio/npc_dialogue/greeting_1.mp3", engine::sound_type::event, "greeting_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/greeting_2.mp3", engine::sound_type::event, "greeting_2");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_1.mp3", engine::sound_type::event, "yes_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_2.mp3", engine::sound_type::event, "yes_2");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_3.1.mp3", engine::sound_type::event, "yes_3");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_4.mp3", engine::sound_type::event, "yes_4");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_1.mp3", engine::sound_type::event, "no_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_2.mp3", engine::sound_type::event, "no_2");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_3.1.mp3", engine::sound_type::event, "no_3");
    mAudioManager->load_sound("assets/audio/npc_dialogue/end_1.mp3", engine::sound_type::event, "end_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/end_2.mp3", engine::sound_type::event, "end_2");


}


void FriendlyNPC::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const {
    glm::mat4 npcTransform(1.0f);
    npcTransform = glm::translate(npcTransform, mObject->position());
    npcTransform = glm::rotate(npcTransform, glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f));
    npcTransform = glm::scale(npcTransform, glm::vec3(0.3f));

    engine::renderer::submit(texturedLightingShader, npcTransform, mObject);

}


void FriendlyNPC::onUpdate(const engine::timestep& timestep, Player& player) {
    if (toPLayDialogue)
        playDialogue();

    // Handling the timer for the shout/grow then attack
    if (mDialogueTimer > 0.f) {
        mDialogueTimer -= static_cast<float>(timestep);
        const float scale = 1.f + timestep; // scalar factor when the enemy grows
        player.setToInteractWithNpc(false);
     }

    player.setDialogueTimer(mDialogueTimer);


    // mObject->animated_mesh()->on_update(timestep);

    // distance relative to player
    const auto distanceFromPLayer = mObject->position() - player.object()->position();
    if(distanceFromPLayer.x<2.f) {
        if(player.getToInteractWithNpc() && mDialogueTimer <=0) {
            playDialogue();
            progress();
            mDialogueTimer = 3.f;
            player.setToInteractWithNpc(false);
        }
    }
}

void FriendlyNPC::playDialogue() const {
    const std::string dialogue = dialoguePath + "_" + std::to_string(dialogueNumber);
    mAudioManager->play(dialogue);
}

void FriendlyNPC::progress() {
    if (dialoguePath == "greeting") {
        if (dialogueNumber == 1)
            dialogueNumber += 1;
        else {
            if (isYes) {
                dialoguePath = "yes";
                dialogueNumber = 1;
            }
            else if (isNo) {
                dialoguePath = "no";
                dialogueNumber = 1;
            }
        }
    }
    else if (dialoguePath == "yes" || dialoguePath == "no") {
        if (dialogueNumber == 1 || dialogueNumber == 2) {
            dialogueNumber += 1;
        }
        else if (isBossBeaten && dialogueNumber == 3) {
            dialoguePath = "end";
            dialogueNumber = 1;
        }

    }
    else if (dialoguePath == "end" && isBossBeaten) {
        if (dialogueNumber == 1) {
            dialogueNumber += 1;
        }
        else if (dialogueNumber == 2) { }
    }
}
