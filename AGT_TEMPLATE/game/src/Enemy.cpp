#include "Enemy.h"


Enemy::Enemy() {
    // Initialise the animated mesh
    enemyMesh = engine::skinned_mesh::create("assets/models/animated/minotaur/Mesh.FBX");
    enemyMesh->LoadAnimationFile("assets/models/animated/minotaur/Minotaur@Idle.FBX");
    enemyMesh->LoadAnimationFile("assets/models/animated/minotaur/Minotaur@Walk.FBX");
    enemyMesh->LoadAnimationFile("assets/models/animated/minotaur/Minotaur@Attack.FBX");
    enemyMesh->LoadAnimationFile("assets/models/animated/minotaur/Minotaur@Death1.FBX");
    enemyMesh->LoadAnimationFile("assets/models/animated/minotaur/Minotaur@Get_Hit.FBX");
    enemyMesh->LoadAnimationFile("assets/models/animated/minotaur/Minotaur@Shout.FBX");
}


Enemy::~Enemy() = default;

void Enemy::initialise() {
    engine::game_object_properties enemyProps;
    enemyProps.animated_mesh = enemyMesh;
    enemyProps.textures = {engine::texture_2d::create("assets/models/animated/minotaur/Minotaur_diffuse.tga", false)};
    enemyProps.type = 0;
    mObject = engine::game_object::create(enemyProps);
    mObject->set_scale(glm::vec3(3.f));
    toTriggerAnimation = true;
}

void Enemy::setRandomPosition() const {
    const float x = (rand() % 100) / 10;
    // const float y = (rand() % 100)/10;
    const float z = (rand() % 100) / 10;

    mObject->set_position(glm::vec3(x, 0.5f, z));
}

void Enemy::setRandomScale() {
    const float scale = (rand() % 30 + 10) / 10;
    mObject->set_scale(glm::vec3(scale));
    maxSize = 2.f * scale;
}

void Enemy::setEnemyType() {
    const int randNum = rand() % 100 + 1;
    if (randNum % TYPE_SHOUTER == 0) {
        enemyType = TYPE_SHOUTER;
    }
    if (randNum % TYPE_TRACKER == 0) {
        enemyType = TYPE_TRACKER;
    }
    if (randNum % TYPE_JUMPER == 0) {
        enemyType = TYPE_JUMPER;
    }

    if (enemyType != 0)
        enemyType = TYPE_WALKER;
}

void Enemy::setEnemyType(int type) {
    enemyType = type;
}

void Enemy::onUpdate(const engine::timestep& timestep, const Player& player) {

    // turn towards the player
    const glm::vec3 v = player.object()->position() - mObject->position();
    const float theta = atan2(v.x, v.z);
    turn(theta);


    // Animation Handler handles the current animation that needs to be played
    animationHandler();

    //check distance from player and stop walking and shout at the player
    auto d = mObject->position() - player.object()->position();
    if (glm::length(d) < 2.f) {
        if (!isAgro) {
            // the animation duration is being returned in frames, so i divided it by 30 to get the answer in seconds as then i can accurately get the timer
            mShoutTimer = static_cast<float>(mObject->animated_mesh()->animations().at(ANIM_SHOUT)->mDuration) / 30.f;
            isAgro = true;
        }
        agro(timestep);
    }
    else {
        // move the enemy
        walk(timestep, player);
    }
    // toTriggerAnimation = true;


    if (mShoutTimer > 0.f) {
        mShoutTimer -= static_cast<float>(timestep);
        float scale = 1.f + timestep;
        if (!((mObject->scale() *= scale).x > maxSize))
            mObject->set_scale(mObject->scale() *= (1.f + timestep));
        // mShoutTimer -= 0.1f;
        if (mShoutTimer <= 0.f) {
            toAttack = true;
        }
    }
    // std::cout << "Shout timer: " << mShoutTimer << "\n";


}


// Handle the animation
void Enemy::animationHandler() {

    // if the current animation is the same as the next animation then do not do anything
    if (currentAnimation == nextAnimation)
        return;

    // based on what the next animation should be, call switch animation method
    switch (nextAnimation) {
    case ANIM_IDLE:
        mObject->animated_mesh()->switch_animation(ANIM_IDLE);
        currentAnimation = nextAnimation;
        break;
    case ANIM_WALK:
        mObject->animated_mesh()->switch_animation(ANIM_WALK);
        currentAnimation = nextAnimation;
        break;
    case ANIM_SHOUT:
        mObject->animated_mesh()->switch_animation(ANIM_SHOUT);
        currentAnimation = nextAnimation;
        break;
    case ANIM_ATTACK:
        mObject->animated_mesh()->switch_animation(ANIM_ATTACK);
        currentAnimation = nextAnimation;
        break;
    }


}

// Turn Enemy towards the player
void Enemy::turn(float angle) {
    mObject->set_rotation_amount(angle);
    angleFromPlayer = angle;
}

void Enemy::idle() {
    nextAnimation = ANIM_IDLE;
}

// walk the enemy in relation to the player
void Enemy::walk(engine::timestep timestep, const Player& player) {
    nextAnimation = ANIM_WALK;
    glm::vec3 directionVector = glm::normalize(player.object()->position() - mObject->position());

    mObject->set_position(mObject->position() += directionVector * glm::vec3(timestep) * calculateSpeed(true));
}

// calculate the speed based on the size of the enemy
float Enemy::calculateSpeed(bool isWalking) {
    if (isWalking)
        return mSpeed / (mObject->scale().x + 2.f);
    else
        return mSpeed;
}

// play the attack animation // will be expanded to contain collisions.
void Enemy::attack() {
    nextAnimation = ANIM_ATTACK;
}

// play the death animation and also will handle turning into a ragdoll
void Enemy::die() {
    nextAnimation = ANIM_DEATH1;
}

// play the shout animation
void Enemy::shout() {
    nextAnimation = ANIM_SHOUT;
}


// if the enmey get close enough to the player, this is called.
void Enemy::agro(const engine::timestep& timestep) {
    //set timer for shout animation and then scale up while that is happening and then start attacking
    if (!toAttack) {
        shout();
    }
    else {
        attack();
    }
}
