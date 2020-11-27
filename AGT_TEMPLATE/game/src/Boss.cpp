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
    bossProps.scale = glm::vec3(2.5f / glm::max(bossModel->size().x, glm::max(bossModel->size().y, bossModel->size().z)));
    // npcProps.scale = glm::vec3(1.5);
    bossProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    // npcProps.bounding_shape = npcMesh->size() / 2.f * npcProps.scale.x;
    // npcProps.textures = { engine::texture_2d::create("assets/models/animated/minotaur/Minotaur_diffuse.tga", false) };
    bossProps.type = 0;
    mObject = engine::game_object::create(bossProps);
    // mObject->set_scale(glm::vec3(3.f));
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));

}

void Boss::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const {


    engine::renderer::submit(texturedLightingShader, mObject);

}


void Boss::onUpdate(const engine::timestep& timestep, Player& player) {
    // turn towards the player
    const glm::vec3 v = player.object()->position() - mObject->position();
    const float theta = atan2(v.x, v.z);
    turn(theta);



}

// Turn Boss towards the player
void Boss::turn(float angle) const {
    mObject->set_rotation_amount(angle);
    // angleFromPlayer = angle;
}

