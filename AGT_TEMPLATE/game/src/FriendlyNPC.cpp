#include "FriendlyNPC.h"

// #include "player.h"

FriendlyNPC::FriendlyNPC() {
    // initialise the audio manager
    mAudioManager = engine::audio_manager::instance();
}

FriendlyNPC::~FriendlyNPC() {}

void FriendlyNPC::initialise() {
    // create the game object
    engine::ref<engine::model> npcModel = engine::model::create("assets/models/static/npc.dae");
    engine::game_object_properties npcProps;
    npcProps.meshes = npcModel->meshes();
    npcProps.scale = glm::vec3(1.f / glm::max(npcModel->size().x, glm::max(npcModel->size().y, npcModel->size().z)));
    npcProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    npcProps.type = 0;
    mObject = engine::game_object::create(npcProps);
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));

    //Audio
    mAudioManager->init();
    // Load the dialogue
    mAudioManager->load_sound("assets/audio/npc_dialogue/greeting_1.mp3", engine::sound_type::event, "greeting_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/greeting_2.mp3", engine::sound_type::event, "greeting_2");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_1.mp3", engine::sound_type::event, "yes_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_2.mp3", engine::sound_type::event, "yes_2");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_3.mp3", engine::sound_type::event, "yes_3");
    mAudioManager->load_sound("assets/audio/npc_dialogue/yes_4.mp3", engine::sound_type::event, "yes_4");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_1.mp3", engine::sound_type::event, "no_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_2.mp3", engine::sound_type::event, "no_2");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_3.mp3", engine::sound_type::event, "no_3");
    mAudioManager->load_sound("assets/audio/npc_dialogue/no_4.mp3", engine::sound_type::event, "no_4");
    mAudioManager->load_sound("assets/audio/npc_dialogue/end_1.mp3", engine::sound_type::event, "end_1");
    mAudioManager->load_sound("assets/audio/npc_dialogue/end_2.mp3", engine::sound_type::event, "end_2");
}

// Render the NPC
void FriendlyNPC::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const {
    glm::mat4 npcTransform(1.0f);
    npcTransform = glm::translate(npcTransform, mObject->position());
    npcTransform = glm::rotate(npcTransform, glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f));
    npcTransform = glm::scale(npcTransform, glm::vec3(0.3f));

    engine::renderer::submit(texturedLightingShader, npcTransform, mObject);
}

// render the Choice HUD
void FriendlyNPC::renderChoiceHUD(engine::ref<engine::text_manager>& textManager) {
    if (toRenderChoiceHUD) {
        // form the text
        const auto yesText = "1: YES";
        const auto noText = "2: NO";

        // Render text
        const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
        textManager->render_text(text_shader, yesText, 1000.f, 100.f, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
        textManager->render_text(text_shader, noText, 1000.f, 25.f, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
    }else if(isInRange) {
        const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
        textManager->render_text(text_shader, "'E' to interact", 950.f, 175.f, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
    }
}

// update the object
void FriendlyNPC::onUpdate(const engine::timestep& timestep, Player& player) {

    if(dialoguePath == "end" &&dialogueNumber == 2) {
        player.recieveApple();
    }

    if (toPLayDialogue)
        playDialogue();

    // Handling the timer for the cooldown of the dialogue button
    if (mDialogueTimer > 0.f) {
        mDialogueTimer -= static_cast<float>(timestep);
    }


    // distance relative to player
    const auto distanceFromPLayer = mObject->position() - player.object()->position();
    isInRange = distanceFromPLayer.x < 1.5f && distanceFromPLayer.z < 1.5f && distanceFromPLayer.x > -1.5f && distanceFromPLayer.z > -1.5f;
    if (isInRange) {
        if (mDialogueTimer <= 0) {
            // play the audio if the E key is pressed
            if (engine::input::key_pressed(engine::key_codes::KEY_E)) {
                playDialogue();
                progress();
                mDialogueTimer = 2.f;
            }
        }
        if (dialogueNumber == 2 && dialoguePath == "greeting" && inChoice) {
            toRenderChoiceHUD = true;
            if (engine::input::key_pressed(engine::key_codes::KEY_1)) {
                isYes = true;
                progress();
                playDialogue();
                progress();
                inChoice = false;
            }
            else if (engine::input::key_pressed(engine::key_codes::KEY_2)) {
                isNo = true;
                progress();
                playDialogue();
                progress();
                inChoice = false;
            }
        }
    }

    if (!isInRange) {
        toRenderChoiceHUD = false;
    }

    if (!(dialogueNumber == 2 && dialoguePath == "greeting"))
        toRenderChoiceHUD = false;

}

// play the appropriate dialogue
void FriendlyNPC::playDialogue() const {
    const std::string dialogue = dialoguePath + "_" + std::to_string(dialogueNumber);
    mAudioManager->play(dialogue);
}

// progress to the next dialogue (or not if requirements are not met)
void FriendlyNPC::progress() {

    if (dialoguePath == "greeting") {
        // progress if 1 then repeat 2 until the player has chosen yes or no
        if (dialogueNumber == 1)
            dialogueNumber += 1;
        else if (dialogueNumber == 2 && (isYes || isNo)) {
            if (isYes)
                dialoguePath = "yes";
            else if (isNo)
                dialoguePath = "no";
            dialogueNumber = 1;
        }
        else if (dialogueNumber == 2) {
            inChoice = true;
        }

    }
    else if (dialoguePath == "yes" || dialoguePath == "no") {
        // progress forward if 1, 2 or 3 then repeat when on dialogue 4 until the boss is beaten
        if (dialogueNumber == 1 || dialogueNumber == 2 || dialogueNumber == 3) {
            dialogueNumber += 1;
        }
        else if (isBossBeaten && dialogueNumber == 4) {
            dialoguePath = "end";
            dialogueNumber = 1;
        }

    }
    else if (dialoguePath == "end" && isBossBeaten) {
        
        if (dialogueNumber == 1) {
            dialogueNumber += 1;
        }
        // do nothing if 2
    }
}
