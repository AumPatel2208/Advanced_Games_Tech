#pragma once
#include <engine.h>


#include "AnimationHandler.h"
#include "player.h"
#include "glm/gtx/rotate_vector.hpp"

class Throwable {


public:
	Throwable();
	~Throwable();
    void onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const;
    void renderPickupHUD(engine::ref<engine::text_manager>& textManager);

    // initialise the Throwable
	void initialise();

	void onUpdate(const engine::timestep& timestep, Player& player);
	engine::ref<engine::game_object> object() const { return mObject; }

	void throwObject(const glm::vec3& playerForward);
	void pickUpObject(const glm::vec3 & playerPosition);

    static bool isInRange(float distanceFromPlayer);

	void calculateSpeed();

private:
	engine::ref<engine::game_object>	mObject;
	float angleFromPlayer = {};
	float mThrowSpeed{ 1.f };
	float mSize = 1.f;
	float pickupRange = 5.f;

	//timer
	float interactTimer = 0.f;
	
	bool isActive = false;
	bool toRenderPickup = false;


	AnimationHandler animationHandler = {};


};

