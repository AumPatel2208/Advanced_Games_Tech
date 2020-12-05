#pragma once
#include <engine.h>


#include "AnimationHandler.h"
#include "player.h"
#include "glm/gtx/rotate_vector.hpp"

class Enemy {

	enum class State
	{
		IDLE,
	    WALK,
		SHOUT,
		ATTACK,
		DIE
	};

public:
	Enemy();
	~Enemy();

	// initialise the enemy
	void initialise();
	void setRandomPosition() const;
	void setRandomScale();
    void setFixedScale();
    void setEnemyType();
	void setEnemyType(int type);
	void onUpdate(const engine::timestep& timestep, Player& player);
	engine::ref<engine::game_object> object() const { return mObject; }

	void turn(float angle);

	void idle();
	void attack();
	void die();
	void walk(engine::timestep timestep, const Player& player);
	float calculateSpeed(bool isWalking) const;
	void shout();

	bool& getIsDead() { return isDead; }

	// Enemy Types based on integer
	static const int TYPE_WALKER = 1;
	static const int TYPE_SHOUTER = 2;
	static const int TYPE_TRACKER = 3;
	static const int TYPE_JUMPER = 5;

private:
	engine::ref<engine::game_object>	mObject;
	engine::ref<engine::skinned_mesh>	enemyMesh;
	int									enemyType = 0;
	float angleFromPlayer = {};
	float mSpeed{ 1.f };
	float mShoutTimer = 0;
	// bool toAttack = false;
	// bool isInShoutRange = false; // amount of times been agroed
	float maxSize = 0.f;
	float agroRange = 5.f;
	State mState = State::IDLE;

	bool isDead = false;

	AnimationHandler animationHandler = {};


};

