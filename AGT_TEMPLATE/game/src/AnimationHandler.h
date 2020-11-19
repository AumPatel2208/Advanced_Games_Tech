#pragma once
#include <engine.h>

class AnimationHandler {
public:
    AnimationHandler(engine::ref<engine::game_object>& object);
    AnimationHandler();
    ~AnimationHandler();

    void onUpdate(const engine::timestep& timestep);
    void nextAnimation(const int& anim) { mNextAnimation = anim; }


    void setIdleAnim(const int animIdle) { ANIM_IDLE = animIdle; }
    void setAnimWalk(const int animWalk) { ANIM_WALK = animWalk; }
    void setAnimAttack(const int animAttack) { ANIM_ATTACK = animAttack; }
    void setAnimDeath(const int animDeath) { ANIM_DEATH = animDeath; }
    void setAnimGetHit(const int animGetHit) { ANIM_GET_HIT = animGetHit; }
    void setAnimShout(const int animShout) { ANIM_SHOUT = animShout; }
    void setAnimJump(const int animJump) { ANIM_JUMP = animJump; }
    void setAnimRun(const int animRun) { ANIM_RUN = animRun; }


    [[nodiscard]] int animIdle() const { return ANIM_IDLE; }
    [[nodiscard]] int animWalk() const { return ANIM_WALK; }
    [[nodiscard]] int animAttack() const { return ANIM_ATTACK; }
    [[nodiscard]] int animDeath() const { return ANIM_DEATH; }
    [[nodiscard]] int animGetHit() const { return ANIM_GET_HIT; }
    [[nodiscard]] int animShout() const { return ANIM_SHOUT; }
    [[nodiscard]] int animJump() const { return ANIM_JUMP; }
    [[nodiscard]] int animRun() const { return ANIM_RUN; }

private:
    //Integer indexes to animations to make it easier to point to the index
    int ANIM_IDLE = {};
    int ANIM_WALK = {};
    int ANIM_ATTACK = {};
    int ANIM_DEATH = {};
    int ANIM_GET_HIT = {};
    int ANIM_SHOUT = {};
    int ANIM_JUMP = {};
    int ANIM_RUN = {};

    int currentAnimation = 0;
    int mNextAnimation = 0;
    engine::ref<engine::game_object> mObject;
};
