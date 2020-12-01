#include "BillboardManager.h"

BillboardManager::BillboardManager() {
    sfx explosion{};
    explosion.billboard = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);
    explosion.height = 2.f;
    explosion.width = 2.f;
    mBillboards.push_back(explosion);
}

BillboardManager::~BillboardManager() {}

void BillboardManager::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader,
                                const engine::perspective_camera& camera) const {
    // Render Billboards
    for (auto& sfx : mBillboards) {
        sfx.billboard->on_render(camera, texturedLightingShader);
    }
}

void BillboardManager::onUpdate(const engine::timestep& timestep) {
    // update Billboards
    for (auto& sfx : mBillboards) {
        sfx.billboard->on_update(timestep);
    }

    if (engine::input::key_pressed(engine::key_codes::KEY_F)) {
        activate(mBillboards.at(sfx_EXPLOSION));
    }

}

void BillboardManager::activate(sfx& sfx) {
    sfx.billboard->activate(sfx.position, sfx.width, sfx.height);
}
