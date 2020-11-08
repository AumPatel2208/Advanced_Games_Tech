#pragma once
#include <engine.h>
#include "player.h"
#include "Enemy.h"

class main_layer : public engine::layer
{
public:
    main_layer();
	~main_layer();

    void on_update(const engine::timestep& timestep) override;
    void initialiseEnemies();
    void updateEnemies(const engine::timestep& timestep);
    void renderEnemies(const std::shared_ptr<engine::shader>& animatedMeshShader);
    void initialisePrimitive();
    void menuCamera();
    void on_render() override;
    void on_event(engine::event& event) override;

private:
	void checkBounce();

	// to check if game has started
	bool hasStarted = false;
	bool menuInHelp = false;
	bool renderLevel1 = false;


    std::string							menuText;

	engine::ref<engine::skybox>			mSkybox{};
	engine::ref<engine::game_object>	mTerrain{};
	engine::ref<engine::game_object>	mCow{};
	engine::ref<engine::game_object>	mTree{};
	engine::ref<engine::game_object>	mBall{};
	engine::ref<engine::game_object>	mMannequin{};
	engine::ref<engine::game_object>	mPrimitive{};
	engine::ref<engine::material>		mMaterial{};
	Player								mPlayer{};
	engine::ref<engine::game_object>	mMenu{};

	//Levels
	std::list<engine::ref<engine::game_object>>	mLevels {};

	//List of enemies
	std::list<Enemy> mEnemies {};


	engine::DirectionalLight            mDirectionalLight;

	std::vector<engine::ref<engine::game_object>>     mGameObjects{};

	engine::ref<engine::bullet_manager> mPhysicsManager{};
	engine::ref<engine::audio_manager>  mAudioManager{};
	float								mPrevSphereYVel = 0.f;
	engine::ref<engine::text_manager>	mTextManager{};

    engine::orthographic_camera       m2DCamera; 
    engine::perspective_camera        m3DCamera;
};
