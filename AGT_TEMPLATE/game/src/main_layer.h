#pragma once
#include <engine.h>


#include "billboard.h"
#include "BillboardManager.h"
#include "Boss.h"
#include "player.h"
#include "Enemy.h"
#include "FriendlyNPC.h"
#include "Throwable.h"

class main_layer : public engine::layer
{
public:
    main_layer();
	~main_layer();

    void on_update(const engine::timestep& timestep) override;
    void initialiseEnemies();
    void updateEnemies(const engine::timestep& timestep);
    void renderEnemies(const std::shared_ptr<engine::shader>& animatedMeshShader);
    void initialiseTetrahedrons(const float& scale, const int& amount);
    void updateTetrahedrons(const engine::timestep& timestep);


	void updateThrowables(const engine::timestep& timestep);
	void renderThrowables(std::shared_ptr<engine::shader> shader);

    void loadMusic();
    void renderMusicHud();
    void changeMusicTrack();
    void renderTetrahedrons(std::shared_ptr<engine::shader> shader);
    void menuCamera();
    void on_render() override;
    void on_event(engine::event& event) override;

	void onCollisions();

private:
	void checkBounce();

	// to check if game has started
	bool hasStarted = false;
	bool menuInHelp = false;
	bool renderLevel1 = false;
    bool showMusicHUD = false;

	float interactionTimer = 0.f;
	float musicMenuTimer = 0.f;


    std::string							menuText;

	engine::ref<engine::skybox>			mSkybox{};
	engine::ref<engine::game_object>	mTerrain{};
	engine::ref<engine::game_object>	mCow{};
	engine::ref<engine::game_object>	mTree{};
	engine::ref<engine::game_object>	mBall{};
	engine::ref<engine::game_object>	mMannequin{};
	std::list<engine::ref<engine::game_object>>	mTetrahedrons{};
	// std::list<engine::ref<engine::game_object>>	mOctahedrons{};
	// engine::ref<engine::game_object>	mOctahedron{};
	engine::ref<engine::material>		mMaterial{};

	// Throwable mThrowable{};
	std::vector<Throwable> mThrowables{};

	Player								mPlayer{};
	engine::ref<engine::game_object>	mMenu{};
    //List of enemies
	std::vector<Enemy>					mEnemies {};
	Boss								mBoss{};
    //NPC Object
	FriendlyNPC mFriendlyNpc {};

    // sfx
	// engine::ref<billboard>				m_billboard{};
	BillboardManager					mBillboardManager{};


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


	// Point lighting
	engine::PointLight m_pointLight;
	uint32_t num_point_lights = 1;

	int playBossMusic = 0;
};
