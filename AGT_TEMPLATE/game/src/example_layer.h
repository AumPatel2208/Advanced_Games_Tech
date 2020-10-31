#pragma once
#include <engine.h>

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& timeStep) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	void checkBounce();

	engine::ref<engine::skybox>			mSkybox{};
	engine::ref<engine::game_object>	mTerrain{};
	engine::ref<engine::game_object>	mCow{};
	engine::ref<engine::game_object>	mTree{};
	engine::ref<engine::game_object>	mBall{};
	engine::ref<engine::game_object>	mMannequin{};

	engine::ref<engine::material>		mMaterial{};

	engine::DirectionalLight            mDirectionalLight;

	std::vector<engine::ref<engine::game_object>>     mGameObjects{};

	engine::ref<engine::bullet_manager> mPhysicsManager{};
	engine::ref<engine::audio_manager>  mAudioManager{};
	float								mPrevSphereYVel = 0.f;
	engine::ref<engine::text_manager>	mTextManager{};

    engine::orthographic_camera       m2DCamera; 
    engine::perspective_camera        m3DCamera;
};
