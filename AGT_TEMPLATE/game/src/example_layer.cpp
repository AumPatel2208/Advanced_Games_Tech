#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include <fstream>


example_layer::example_layer()
    : m2DCamera(-1.6f, 1.6f, -0.9f, 0.9f),
      m3DCamera((float)engine::application::window().width(), (float)engine::application::window().height()) {
    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

    // Load File
    std::ifstream inFile;
    // inFile.open("C:\\Users\\Aum\\Documents\\Games Tech\\Coursework\\AGT_TEMPLATE\\game\\assets\\text\\menu.txt");
    inFile.open("assets/text/menu.txt");


    if (!inFile) {
        std::cerr << "Unable to open file datafile.txt";
        exit(1); // call system to stop
    }
    char x;
    while (inFile >> x) {
        menuText = menuText + x;
    }
    std::cout << menuText;
    inFile.close();


    // Place the camera birds eye
    // m3DCamera.position(glm::vec3(0.f, 5.f, 0.f));
    // m3DCamera.set_view_matrix_custom(m3DCamera.position(), glm::vec3(0, -2.f, 0));


    // Initialise audio and play background music
    mAudioManager = engine::audio_manager::instance();
    mAudioManager->init();
    mAudioManager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce");
    // Royalty free sound from freesound.org
    mAudioManager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");
    // Royalty free music from http://www.nosoapradio.us/
    //m_audio_manager->play("music");


    // Initialise the shaders, materials and lights
    auto meshMaterialShader = engine::renderer::shaders_library()->get("mesh_material");
    auto meshLightingShader = engine::renderer::shaders_library()->get("mesh_lighting");
    auto textShader = engine::renderer::shaders_library()->get("text_2D");
    auto animatedMeshShader = engine::renderer::shaders_library()->get("animated_mesh");

    mDirectionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
    mDirectionalLight.AmbientIntensity = 0.25f;
    mDirectionalLight.DiffuseIntensity = 0.6f;
    mDirectionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

    // set color texture unit
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform("gColorMap", 0);
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform("lighting_on", true);
    mDirectionalLight.submit(animatedMeshShader);
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform("gMatSpecularIntensity", 0.5f);
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform("gSpecularPower", 5.f);
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform("transparency", 1.0f);
    std::dynamic_pointer_cast<engine::gl_shader>(meshMaterialShader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(meshMaterialShader)->set_uniform("lighting_on", true);
    mDirectionalLight.submit(meshMaterialShader);
    std::dynamic_pointer_cast<engine::gl_shader>(meshMaterialShader)->set_uniform("gMatSpecularIntensity", 1.f);
    std::dynamic_pointer_cast<engine::gl_shader>(meshMaterialShader)->set_uniform("gSpecularPower", 10.f);
    std::dynamic_pointer_cast<engine::gl_shader>(meshLightingShader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(meshLightingShader)->set_uniform("gColorMap", 0);
    std::dynamic_pointer_cast<engine::gl_shader>(meshLightingShader)->set_uniform("lighting_on", true);
    mDirectionalLight.submit(meshLightingShader);
    std::dynamic_pointer_cast<engine::gl_shader>(meshLightingShader)->set_uniform("gMatSpecularIntensity", 0.5f);
    std::dynamic_pointer_cast<engine::gl_shader>(meshLightingShader)->set_uniform("gSpecularPower", 5.f);
    std::dynamic_pointer_cast<engine::gl_shader>(meshLightingShader)->set_uniform("transparency", 1.0f);
    std::dynamic_pointer_cast<engine::gl_shader>(textShader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(textShader)->set_uniform("projection",
                                                                          glm::ortho(
                                                                              0.f, (float)engine::application::window()
                                                                              .width(), 0.f,
                                                                              (float)engine::application::window().
                                                                              height()));


    engine::ref<engine::game_object> level1{};
    engine::game_object_properties level1Props;
    engine::ref<engine::model> level1Model = engine::model::create("assets/levels/level_1.dae");

    level1Props.position = glm::vec3(0.f, 0.f, 0.f);
    level1Props.meshes = level1Model->meshes();
    level1Props.scale = glm::vec3(1.f);
    level1 = engine::game_object::create(level1Props);
    mLevels.push_back(level1);


    mMaterial = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
                                         glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);


    // Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
    mSkybox = engine::skybox::create(50.f,
                                     {
                                         engine::texture_2d::create("assets/textures/skybox/mountain1/front.jpg",
                                                                    true),
                                         engine::texture_2d::create("assets/textures/skybox/mountain1/right.jpg",
                                                                    true),
                                         engine::texture_2d::create("assets/textures/skybox/mountain1/back.jpg", true),
                                         engine::texture_2d::create("assets/textures/skybox/mountain1/left.jpg", true),
                                         engine::texture_2d::create("assets/textures/skybox/mountain1/top.jpg", true),
                                         engine::texture_2d::create("assets/textures/skybox/mountain1/bottom.jpg",
                                                                    true)
                                     });

    engine::ref<engine::skinned_mesh> mSkinnedMesh = engine::skinned_mesh::create(
        "assets/models/animated/mannequin/free3Dmodel.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
    mSkinnedMesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
    mSkinnedMesh->switch_root_movement(false);

    engine::game_object_properties mannequinProps;
    mannequinProps.animated_mesh = mSkinnedMesh;
    mannequinProps.scale = glm::vec3(1.f / glm::max(mSkinnedMesh->size().x,
                                                    glm::max(mSkinnedMesh->size().y, mSkinnedMesh->size().z)));
    mannequinProps.position = glm::vec3(3.0f, 0.5f, -5.0f);
    mannequinProps.type = 0;
    mannequinProps.bounding_shape = mSkinnedMesh->size() / 2.f * mannequinProps.scale.x;

    mMannequin = engine::game_object::create(mannequinProps);
    mPlayer.initialise(mMannequin);


    // Enemy tempEnemy = {};
    // tempEnemy.initialise();
    //
    // mEnemies.push_back(tempEnemy.object());

    // initialise Enemies
    initialiseEnemies();


    // Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
    const std::vector<engine::ref<engine::texture_2d>> terrainTextures = {
        engine::texture_2d::create("assets/textures/sea_rock_terrain.jpg", false)
    };
    const engine::ref<engine::terrain> terrainShape = engine::terrain::create(100.f, 0.5f, 100.f);
    engine::game_object_properties terrainProps;
    terrainProps.meshes = {terrainShape->mesh()};
    terrainProps.textures = terrainTextures;
    terrainProps.is_static = true;
    terrainProps.type = 0;
    terrainProps.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
    terrainProps.restitution = 0.92f;
    mTerrain = engine::game_object::create(terrainProps);

    // Load the cow model. Create a cow object. Set its properties
    engine::ref<engine::model> cowModel = engine::model::create("assets/models/static/cow4.3ds");
    engine::game_object_properties cow_props;
    cow_props.meshes = cowModel->meshes();
    cow_props.textures = cowModel->textures();
    float cowScale = 1.f / glm::max(cowModel->size().x, glm::max(cowModel->size().y, cowModel->size().z));
    cow_props.position = {-4.f, 0.5f, -5.f};
    cow_props.scale = glm::vec3(cowScale);
    cow_props.bounding_shape = cowModel->size() / 2.f * cowScale;
    mCow = engine::game_object::create(cow_props);
    
    // Load the tree model. Create a tree object. Set its properties
    engine::ref<engine::model> treeModel = engine::model::create("assets/models/static/elm.3ds");
    engine::game_object_properties tree_props;
    tree_props.meshes = treeModel->meshes();
    tree_props.textures = treeModel->textures();
    const auto treeScale = 3.f / glm::max(treeModel->size().x, glm::max(treeModel->size().y, treeModel->size().z));
    tree_props.position = {4.f, 0.5f, -5.f};
    tree_props.bounding_shape = treeModel->size() / 2.f * treeScale;
    tree_props.scale = glm::vec3(treeScale);
    mTree = engine::game_object::create(tree_props);

    const engine::ref<engine::sphere> sphereShape = engine::sphere::create(10, 20, 0.5f);
    engine::game_object_properties sphere_props;
    sphere_props.position = {0.f, 5.f, -5.f};
    sphere_props.meshes = {sphereShape->mesh()};
    sphere_props.type = 1;
    sphere_props.bounding_shape = glm::vec3(0.5f);
    sphere_props.restitution = 0.92f;
    sphere_props.mass = 0.000001f;
    mBall = engine::game_object::create(sphere_props);

    const engine::ref<engine::cuboid> menuShape = engine::cuboid::create(
        glm::vec3((float)engine::application::window().width() / 500, 0.1f,
                  (float)engine::application::window().height() / 500), false);
    auto menuTexture = engine::texture_2d::create("assets/textures/menu.png", true);
    auto helpTexture = engine::texture_2d::create("assets/textures/help.png", true);
    engine::game_object_properties menuProps;
    menuProps.position = {0.f, 0.f, 0.f};
    menuProps.textures = {menuTexture, helpTexture};
    menuProps.meshes = {menuShape->mesh()};
    mMenu = engine::game_object::create(menuProps);





    mGameObjects.push_back(mTerrain);
    mGameObjects.push_back(mBall);
    // mGameObjects.push_back(mLevels.front());

    //m_game_objects.push_back(m_cow);
    //m_game_objects.push_back(m_tree);
    //m_game_objects.push_back(m_pickup);
    mPhysicsManager = engine::bullet_manager::create(mGameObjects);

    mTextManager = engine::text_manager::create();

    // mSkinnedMesh->switch_animation(1);
    mPlayer.object()->animated_mesh()->switch_animation(1);

    // for (auto& enemy: mEnemies) {
    //     enemy.object()->animated_mesh()->switch_animation(1);
    // }
    // mEnemies.front()->animated_mesh()->switch_animation(3);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& timestep) {

    if (hasStarted) {
        mPhysicsManager->dynamics_world_update(mGameObjects, double(timestep));

        //Free flowing camera
        // m3DCamera.on_update(timestep);
        // mMannequin->animated_mesh()->on_update(timeStep);
        mPlayer.onUpdate(timestep);

        mPlayer.updateCamera(m3DCamera, timestep);

        // Put the player into the level that already exists
        if (engine::input::key_pressed(engine::key_codes::KEY_P)) {
            mPlayer.moveIntoLevel1();
        }

        updateEnemies(timestep);
    }
    else {
        // set the camera on the menu if the game hasn't started yet
        menuCamera();
        if (engine::input::key_pressed(engine::key_codes::KEY_1)) {
            hasStarted = true;
            mPlayer.setHasStarted(true);
            // m3DCamera.position(glm::vec3(0.f, 1.f, 0.f));
            //SET TIMER TO TRANSITION HERE
        }
        else if (engine::input::key_pressed(engine::key_codes::KEY_2)) {
            if (!menuInHelp) {
                menuInHelp = true;
            }
        }
        if(menuInHelp) {
            if (engine::input::key_pressed(engine::key_codes::KEY_BACKSPACE)) {
                menuInHelp = false;
            }
        }
        
    }


    checkBounce();
}

void example_layer::initialiseEnemies() {
    int numberOfEnemies = 3;
    for (int i = 0; i < numberOfEnemies; ++i) {
        Enemy enemy = {};
        enemy.initialise();
        enemy.setRandomPosition();
        enemy.setRandomScale();
        mEnemies.push_back(enemy);
    }
}

void example_layer::updateEnemies(const engine::timestep& timestep) {
    for (auto& enemy: mEnemies) {
        enemy.onUpdate(timestep, mPlayer);
        enemy.object()->animated_mesh()->on_update(timestep);
    }

}

void example_layer::renderEnemies(const std::shared_ptr<engine::shader> &animatedMeshShader) {
    for (auto& enemy : mEnemies) {
        engine::renderer::submit(animatedMeshShader, enemy.object());
    }
}


// Set the camera to display the menu
void example_layer::menuCamera() {
    float A = 1.f;
    float B = -3.f;
    float C = 6.f;

    auto menuForward = glm::vec3(0.f, 0.f, -1.f);
    auto menuPosition = glm::vec3(0, 15.5f, 0);

    menuForward = (glm::rotate(menuForward, glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f)));

    menuForward = glm::normalize(menuForward);

    glm::vec3 camPos = menuPosition - menuForward * B;
    camPos.y += A;

    glm::vec3 camLookAt = menuPosition + menuForward * C;
    camLookAt.y = 0.f;
    // camera.set_view_matrix_custom(cam_pos, cam_look_at, mObject->position(), timeStep);
    m3DCamera.set_view_matrix(camPos, camLookAt);
}

void example_layer::on_render() {
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f});
    engine::render_command::clear();

    //const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
    //engine::renderer::begin_scene(m_3d_camera, textured_shader);

    const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
    engine::renderer::begin_scene(m3DCamera, textured_lighting_shader);

    // Set up some of the scene's parameters in the shader
    std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform(
        "gEyeWorldPos", m3DCamera.position());

    // Position the skybox centred on the player and render it
    glm::mat4 skyboxTransform(1.0f);
    skyboxTransform = glm::translate(skyboxTransform, m3DCamera.position());
    for (const auto& texture : mSkybox->textures()) {
        texture->bind();
    }
    engine::renderer::submit(textured_lighting_shader, mSkybox, skyboxTransform);

    engine::renderer::submit(textured_lighting_shader, mTerrain);

    
    glm::mat4 treeTransform(1.0f);
    treeTransform = glm::translate(treeTransform, glm::vec3(4.f, 0.5, -5.0f));
    treeTransform = glm::rotate(treeTransform, mTree->rotation_amount(), mTree->rotation_axis());
    treeTransform = glm::scale(treeTransform, mTree->scale());
    engine::renderer::submit(textured_lighting_shader, treeTransform, mTree);
    
    glm::mat4 cowTransform(1.0f);
    cowTransform = glm::translate(cowTransform, mCow->position());
    cowTransform = glm::rotate(cowTransform, mCow->rotation_amount(), mCow->rotation_axis());
    cowTransform = glm::scale(cowTransform, mCow->scale());
    engine::renderer::submit(textured_lighting_shader, cowTransform, mCow);

    glm::mat4 level1Transform(1.f);
    level1Transform = glm::translate(level1Transform, glm::vec3(0.f, 2.9f, 0.f));
    level1Transform = glm::rotate(level1Transform, glm::radians(270.f), glm::vec3(1.f,0.f,0.f));
    level1Transform = glm::scale(level1Transform, glm::vec3(2.5f));

    engine::renderer::submit(textured_lighting_shader,level1Transform, mLevels.front());

    if (!hasStarted) {
        //render menu
        if (menuInHelp) {
            mMenu->textures().at(1)->bind();
        }
        else {
            mMenu->textures().at(0)->bind();
        }
        glm::mat4 menuTransform(1.0f);
        menuTransform = glm::translate(menuTransform, glm::vec3(0, 10.f, 0));

        engine::renderer::submit(textured_lighting_shader, mMenu->meshes().at(0),
            menuTransform);
    }


    engine::renderer::end_scene();

    // Set up material shader. (does not render textures, renders materials instead)
    const auto materialShader = engine::renderer::shaders_library()->get("mesh_material");
    engine::renderer::begin_scene(m3DCamera, materialShader);

    mMaterial->submit(materialShader);
    std::dynamic_pointer_cast<engine::gl_shader>(materialShader)->set_uniform("gEyeWorldPos", m3DCamera.position());

    engine::renderer::submit(materialShader, mBall);

    engine::renderer::end_scene();


    
    const auto animatedMeshShader = engine::renderer::shaders_library()->get("animated_mesh");
    engine::renderer::begin_scene(m3DCamera, animatedMeshShader);
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform(
        "gEyeWorldPos", m3DCamera.position());

    // glm::mat4 aniTransform = glm::mat4(1.0f);
    // aniTransform = glm::translate(glm::vec3(0.f,1.f,0.f));
    engine::renderer::submit(animatedMeshShader, mPlayer.object());
    // engine::renderer::submit(animatedMeshShader, mMannequin);

    glm::mat4 enemyTransform = glm::mat4(1.f);
    enemyTransform = glm::translate(enemyTransform, glm::vec3(0.f, .5f, 0.f));
    // enemyTransform = glm::rotate(enemyTransform, glm::radians(270.f), glm::vec3(1.f,0.f,0.f));
    // enemyTransform = glm::scale(enemyTransform, glm::vec3(10.f));
    // engine::renderer::submit(animatedMeshShader, enemyTransform, mEnemies.front());

    renderEnemies(animatedMeshShader);

    engine::renderer::end_scene();

}

void example_layer::on_event(engine::event& event) {
    if (event.event_type() == engine::event_type_e::key_pressed) {
        auto& e = dynamic_cast<engine::key_pressed_event&>(event);
        if (e.key_code() == engine::key_codes::KEY_TAB) {
            engine::render_command::toggle_wireframe();
        }
    }
}

void example_layer::checkBounce() {
    //if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
    //m_audio_manager->play("bounce");
    mPrevSphereYVel = mGameObjects.at(1)->velocity().y;
}
