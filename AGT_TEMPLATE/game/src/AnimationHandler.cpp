#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(engine::ref<engine::game_object> object): mObject(object){}
AnimationHandler::AnimationHandler() {};

AnimationHandler::~AnimationHandler() {
    
}

void AnimationHandler::onUpdate(const engine::timestep& timestep) {
    mObject->animated_mesh()->on_update(timestep);

    // if the current animation is the same as the next animation then do not do anything
    if (currentAnimation == mNextAnimation)
        return;

    // based on what the next animation should be, call switch animation method
    switch (mNextAnimation) {
    case ANIM_IDLE:
        mObject->animated_mesh()->switch_animation(ANIM_IDLE);
        currentAnimation = mNextAnimation;
        break;
    case ANIM_WALK:
        mObject->animated_mesh()->switch_animation(ANIM_WALK);
        currentAnimation = mNextAnimation;
        break;
    case ANIM_SHOUT:
        mObject->animated_mesh()->switch_animation(ANIM_SHOUT);
        currentAnimation = mNextAnimation;
        break;
    case ANIM_ATTACK:
        mObject->animated_mesh()->switch_animation(ANIM_ATTACK);
        currentAnimation = mNextAnimation;
        break;
    default:
        break;
    }


}

