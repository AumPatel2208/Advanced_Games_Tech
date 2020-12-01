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
    bossProps.scale = glm::vec3(
        2.5f / glm::max(bossModel->size().x, glm::max(bossModel->size().y, bossModel->size().z)));
    // npcProps.scale = glm::vec3(1.5);
    bossProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    // npcProps.bounding_shape = npcMesh->size() / 2.f * npcProps.scale.x;
    // npcProps.textures = { engine::texture_2d::create("assets/models/animated/minotaur/Minotaur_diffuse.tga", false) };
    bossProps.type = 0;
    bossProps.textures = {engine::texture_2d::create("assets/textures/metal_bullet_2.jpg", false)};
    mObject = engine::game_object::create(bossProps);
    // mObject->set_scale(glm::vec3(3.f));
    mObject->set_forward(glm::vec3(0.f, 0.f, -1.f));
    mObject->set_position(glm::vec3(0.f, 0.5f, 10.f));

    //Bullet shape
    const engine::ref<engine::BulletShape> bulletShape = engine::BulletShape::createDefaultVertices(0.25f, 0.1f, 0.1f);
    const std::vector<engine::ref<engine::texture_2d>> bulletTextures = {
        engine::texture_2d::create("assets/textures/metal_bullet.jpg", false)
    };
    engine::game_object_properties bulletProps;
    bulletProps.position = {mObject->position().x, mObject->position().y + 1.f, mObject->position().z};
    bulletProps.meshes = {bulletShape->mesh()};
    bulletProps.textures = bulletTextures;
    auto bulletObject = engine::game_object::create(bulletProps);
    mBullet = bulletObject;
    // mBullet->set_rotation_amount(glm::radians(90.f));
}

void Boss::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader,
                    const engine::perspective_camera& camera) const {

    if (showBullet) {
        engine::renderer::submit(texturedLightingShader, mBullet);
    }

    engine::renderer::submit(texturedLightingShader, mObject);
}

void Boss::moveBullet() { }

void Boss::onUpdate(const engine::timestep& timestep, Player& player, BillboardManager& billboardManager) {
    // turn towards the player
    const glm::vec3 v = player.object()->position() - mObject->position();
    const float theta = atan2(v.x, v.z);
    turn(theta);

    billboardManager.getSfx(BillboardManager::sfx_EXPLOSION).position = glm::vec3(
        mObject->position().x, mObject->position().y + 0.5f, mObject->position().z + 0.1f);


    if (engine::input::key_pressed(engine::key_codes::KEY_F)) {
        if (bulletTimer <= 0) {
            bulletTrajectory = glm::normalize(player.object()->position() - mObject->position());
            shoot(billboardManager);
        }
    }

    // Handling the timer for the bullet shooting
    if (bulletTimer > 0.f) {
        mBullet->set_position(mBullet->position() += bulletTrajectory * glm::vec3(timestep) * 2.f);
        bulletTimer -= static_cast<float>(timestep);
        if (bulletTimer <= 0.f) {
            showBullet = false;
            mBullet->set_position(glm::vec3(mObject->position().x, mObject->position().y + 1.f, mObject->position().z));

            // do another explosion
        }
    }
}

// Turn Boss towards the player
void Boss::turn(float angle) const {
    mObject->set_rotation_amount(angle);
    if (bulletTimer <= 0)
        mBullet->set_rotation_amount(angle);
}

void Boss::shoot(BillboardManager& billboardManager) {
    billboardManager.activate(billboardManager.getSfx(BillboardManager::sfx_EXPLOSION));
    showBullet = true;
    bulletTimer = 2.5f;
}
