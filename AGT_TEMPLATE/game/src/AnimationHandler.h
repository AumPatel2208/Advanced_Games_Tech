#pragma once
#include <engine.h>

class AnimationHandler {
public:
	AnimationHandler(	engine::ref<engine::game_object> object);
	AnimationHandler(	);
    ~AnimationHandler();

    void onUpdate(const engine::timestep& timestep);

	//Static references to animations to make it easier to point to the index
	static const int ANIM_IDLE = 0;
	static const int ANIM_WALK = 1;
	static const int ANIM_ATTACK = 2;
	static const int ANIM_DEATH = 3;
	static const int ANIM_GET_HIT = 4;
	static const int ANIM_SHOUT = 5;

	void nextAnimation(const int& anim) { mNextAnimation = anim; }
	

private:
	int currentAnimation = ANIM_IDLE;
	int mNextAnimation = ANIM_IDLE;
	engine::ref<engine::game_object>	mObject;
};
