#pragma once
#include <engine.h>

#include "Boss.h"
#include "player.h"
#include "Enemy.h"
#include "FriendlyNPC.h"

class main_layer : public engine::layer
{
public:
    main_layer();
	~main_layer();

    void on_update(const engine::timestep& timestep) override;
    void initialiseEnemies();
    void updateEnemies(const engine::timestep& timestep);
    void renderEnemies(const std::shared_ptr<engine::shader>& animatedMeshShader);
    void initialiseTetrhedrons(const float& scale, const int& amount);
    void updatePrimitives(const engine::timestep& timestep);
    void loadMusic();
    void renderMusicHud();
    void changeMusicTrack();
    void renderPrimitives(std::shared_ptr<engine::shader> shader);
    void menuCamera();
    void on_render() override;
    void on_event(engine::event& event) override;

private:
	void checkBounce();

	// to check if game has started
	bool hasStarted = false;
	bool menuInHelp = false;
	bool renderLevel1 = false;
    bool showMusicHUD = false;


    std::string							menuText;

	engine::ref<engine::skybox>			mSkybox{};
	engine::ref<engine::game_object>	mTerrain{};
	engine::ref<engine::game_object>	mCow{};
	engine::ref<engine::game_object>	mTree{};
	engine::ref<engine::game_object>	mBall{};
	engine::ref<engine::game_object>	mMannequin{};
	std::list<engine::ref<engine::game_object>>	mTetrahedrons{};
	std::list<engine::ref<engine::game_object>>	mOctahedrons{};
	engine::ref<engine::game_object>	mOctahedron{};
	engine::ref<engine::material>		mMaterial{};
	Player								mPlayer{};
	engine::ref<engine::game_object>	mMenu{};
    //List of enemies
	std::list<Enemy>					mEnemies {};
	Boss								mBoss{};
    //NPC Object
	FriendlyNPC mFriendlyNpc {};




	// Max/Min for primitive
	float maxPrimSize = 1.2f;
	float minPrimSize = 0.8f;
	bool isPrimGrowing = true;

	//Levels
	std::list<engine::ref<engine::game_object>>	mLevels {};

		engine::DirectionalLight            mDirectionalLight;

	std::vector<engine::ref<engine::game_object>>     mGameObjects{};

	engine::ref<engine::bullet_manager> mPhysicsManager{};
	engine::ref<engine::audio_manager>  mAudioManager{};
	std::vector<std::string> mMusicFileNames{};

	float								mPrevSphereYVel = 0.f;
	engine::ref<engine::text_manager>	mTextManager{};

    engine::orthographic_camera       m2DCamera; 
    engine::perspective_camera        m3DCamera;
};
