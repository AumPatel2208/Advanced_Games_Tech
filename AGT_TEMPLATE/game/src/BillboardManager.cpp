#include "BillboardManager.h"

BillboardManager::BillboardManager() {
    // create the many billboards that will be used in the game

    // create the explosion billboard
    sfx explosion{};
    explosion.billboard = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);
    explosion.height = 2.f;
    explosion.width = 2.f;
    mBillboards.push_back(explosion);

    // create the blood billboard
    // unfinished due to time constraints
    sfx blood1{};
}

BillboardManager::~BillboardManager() {}

// cycle through the list of billboards and render them onto the screen
void BillboardManager::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader,
                                const engine::perspective_camera& camera) const {
    // Render Billboards
    for (auto& sfx : mBillboards) {
        sfx.billboard->on_render(camera, texturedLightingShader);
    }
}

// update the billboards
void BillboardManager::onUpdate(const engine::timestep& timestep) {
    // update Billboards
    for (auto& sfx : mBillboards) {
        sfx.billboard->on_update(timestep);
    }

    if (engine::input::key_pressed(engine::key_codes::KEY_F)) {
        // activate(mBillboards.at(sfx_EXPLOSION));
    }

}

// activate the billboard specified
void BillboardManager::activate(sfx& sfx) {
    sfx.billboard->activate(sfx.position, sfx.width, sfx.height);
}
