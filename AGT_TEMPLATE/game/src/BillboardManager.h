#pragma once
#include <engine.h>
#include "billboard.h"

class BillboardManager {
public:
    struct sfx {
        engine::ref<billboard> billboard{};
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        float width = 1.f;
        float height = 1.f;
    };
    BillboardManager();
    ~BillboardManager();

    void onRender(const std::shared_ptr<engine::shader>& texturedLightingShader, const engine::perspective_camera& camera) const;
    void onUpdate(const engine::timestep& timestep);
    void activate(sfx& sfx);

    sfx& getSfx(int position) {
        return mBillboards.at(position);
    }

    static const int sfx_EXPLOSION = 0;

private:
    std::vector<sfx> mBillboards{};
	// engine::ref<billboard> mExplosion{};
	// sfx mExplosion{};

};

