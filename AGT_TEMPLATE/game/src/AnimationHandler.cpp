#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(engine::ref<engine::game_object>& object): mObject(object) {}
AnimationHandler::AnimationHandler() {};

AnimationHandler::~AnimationHandler() { }

void AnimationHandler::onUpdate(const engine::timestep& timestep) {
    mObject->animated_mesh()->on_update(timestep);

    // if the current animation is the same as the next animation then do not do anything
    if (currentAnimation == mNextAnimation)
        return;

    if(mNextAnimation ==  ANIM_IDLE) {
        mObject->animated_mesh()->switch_animation(ANIM_IDLE);
        currentAnimation = mNextAnimation;
    }else if(mNextAnimation == ANIM_WALK) {
        mObject->animated_mesh()->switch_animation(ANIM_WALK);
        currentAnimation = mNextAnimation;
    }
    else if (mNextAnimation == ANIM_SHOUT) {
        mObject->animated_mesh()->switch_animation(ANIM_SHOUT);
        currentAnimation = mNextAnimation;
    }
    else if (mNextAnimation == ANIM_ATTACK) {
        mObject->animated_mesh()->switch_animation(ANIM_ATTACK);
        currentAnimation = mNextAnimation;
    }
    else if (mNextAnimation == ANIM_RUN) {
            mObject->animated_mesh()->switch_animation(ANIM_RUN);
            currentAnimation = mNextAnimation;
    }
    else if (mNextAnimation == ANIM_DEATH) {
        mObject->animated_mesh()->switch_animation(ANIM_DEATH);
        currentAnimation = mNextAnimation;
    }
    else if (mNextAnimation == ANIM_JUMP) {
        mObject->animated_mesh()->switch_animation(ANIM_JUMP);
        currentAnimation = mNextAnimation;
    }
    else if (mNextAnimation == ANIM_GET_HIT) {
        mObject->animated_mesh()->switch_animation(ANIM_GET_HIT);
        currentAnimation = mNextAnimation;
    }
        
    

    // // based on what the next animation should be, call switch animation method
    // switch (mNextAnimation) {
    // case animIdle():
    //     mObject->animated_mesh()->switch_animation(ANIM_IDLE);
    //     currentAnimation = mNextAnimation;
    //     break;
    // case ANIM_WALK:
    //     mObject->animated_mesh()->switch_animation(ANIM_WALK);
    //     currentAnimation = mNextAnimation;
    //     break;
    // case ANIM_SHOUT:
    //     mObject->animated_mesh()->switch_animation(ANIM_SHOUT);
    //     currentAnimation = mNextAnimation;
    //     break;
    // case ANIM_ATTACK:
    //     mObject->animated_mesh()->switch_animation(ANIM_ATTACK);
    //     currentAnimation = mNextAnimation;
    //     break;
    // default:
    //     break;
    // }


}
