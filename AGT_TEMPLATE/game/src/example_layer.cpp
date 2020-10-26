#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "Pickup.h"

example_layer::example_layer()
    : m2DCamera(-1.6f, 1.6f, -0.9f, 0.9f),
      m3DCamera((float)engine::application::window().width(), (float)engine::application::window().height()) {

    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

    // Initialise audio and play background music
    mAudioManager = engine::audio_manager::instance();
    mAudioManager->init();
    mAudioManager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce");
    // Royalty free sound from freesound.org
    mAudioManager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");
    // mAudioManager->load_sound("assets/audio/wow.mp3", engine::sound_type::event, "bounce");
    // Royalty free music from http://www.nosoapradio.us/
    //m_audio_manager->play("music");
    // mAudioManager->play("bounce");

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
    mMaterial = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
                                          glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);


    // Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
    mSkybox = engine::skybox::create(50.f,
                                     {
                                         engine::texture_2d::create("assets/textures/skybox/SkyboxFront.bmp", true),
                                         engine::texture_2d::create("assets/textures/skybox/SkyboxRight.bmp", true),
                                         engine::texture_2d::create("assets/textures/skybox/SkyboxBack.bmp", true),
                                         engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.bmp", true),
                                         engine::texture_2d::create("assets/textures/skybox/SkyboxTop.bmp", true),
                                         engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.bmp", true)
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

    // Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
    std::vector<engine::ref<engine::texture_2d>> terrainTextures = {
        engine::texture_2d::create("assets/textures/terrain.bmp", false)
    };
    engine::ref<engine::terrain> terrainShape = engine::terrain::create(100.f, 0.5f, 100.f);
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
    engine::game_object_properties cowProps;
    cowProps.meshes = cowModel->meshes();
    cowProps.textures = cowModel->textures();
    float cowScale = 1.f / glm::max(cowModel->size().x, glm::max(cowModel->size().y, cowModel->size().z));
    cowProps.position = {-4.f, 0.5f, -5.f};
    cowProps.scale = glm::vec3(cowScale);
    cowProps.bounding_shape = cowModel->size() / 2.f * cowScale;
    mCow = engine::game_object::create(cowProps);

    // Load the tree model. Create a tree object. Set its properties
    engine::ref<engine::model> treeModel = engine::model::create("assets/models/static/elm.3ds");
    engine::game_object_properties tree_props;
    tree_props.meshes = treeModel->meshes();
    tree_props.textures = treeModel->textures();
    float treeScale = 3.f / glm::max(treeModel->size().x, glm::max(treeModel->size().y, treeModel->size().z));
    tree_props.position = {4.f, 0.5f, -5.f};
    tree_props.bounding_shape = treeModel->size() / 2.f * treeScale;
    tree_props.scale = glm::vec3(treeScale);
    mTree = engine::game_object::create(tree_props);

    engine::ref<engine::sphere> sphereShape = engine::sphere::create(10, 20, 0.5f);
    engine::game_object_properties sphere_props;
    sphere_props.position = {0.f, 5.f, -5.f};
    sphere_props.meshes = {sphereShape->mesh()};
    sphere_props.type = 1;
    sphere_props.bounding_shape = glm::vec3(0.5f);
    sphere_props.restitution = 0.92f;
    sphere_props.mass = 0.000001f;
    mBall = engine::game_object::create(sphere_props);

    std::vector<glm::vec3> tetrahedronVertices;
    tetrahedronVertices.push_back(glm::vec3(0.f, 10.f, 0.f));//0
    tetrahedronVertices.push_back(glm::vec3(0.f, 0.f, 10.f));//1
    tetrahedronVertices.push_back(glm::vec3(-10.f, 0.f, -10.f)); //2
    tetrahedronVertices.push_back(glm::vec3(10.f, 0.f, -10.f)); //3 
    engine::ref<engine::Tetrahedron> tetrahedronShape = engine::Tetrahedron::create(tetrahedronVertices);
    engine::game_object_properties tetrahedronProperties;
    tetrahedronProperties.position = { 0.f,0.5f,-20.f };
    tetrahedronProperties.meshes = { tetrahedronShape->getMesh() };
    mTetrahedron = engine::game_object::create(tetrahedronProperties);





    engine::ref<engine::cuboid> pickupShape = engine::cuboid::create(glm::vec3(0.5f), false);
    engine::ref<engine::texture_2d> pickupTexture = engine::texture_2d::create("assets/textures/medkit.jpg", true);
    engine::game_object_properties pickupProps;
    pickupProps.position = {5.f, 1.f, 5.f};
    pickupProps.meshes = {pickupShape->mesh()};
    pickupProps.textures = {pickupTexture};
    mPickup = pickup::create(pickupProps);
    mPickup->init();


    mGameObjects.push_back(mTerrain);
    mGameObjects.push_back(mBall);
    
    // mGameObjects.push_back(mCow);
    //m_game_objects.push_back(m_tree);
    //m_game_objects.push_back(m_pickup);


    mPhysicsManager = engine::bullet_manager::create(mGameObjects);

    

    mTextManager = engine::text_manager::create();

    mSkinnedMesh->switch_animation(1);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) {
    m3DCamera.on_update(time_step);

    mPhysicsManager->dynamics_world_update(mGameObjects, double(time_step));

   

    mMannequin->animated_mesh()->on_update(time_step);

    mPickup->update(m3DCamera.position(), 1.f);

    checkBounce();
}

void example_layer::on_render() {
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f});
    engine::render_command::clear();

    //const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
    //engine::renderer::begin_scene(m_3d_camera, textured_shader);

    const auto texturedLightingShader = engine::renderer::shaders_library()->get("mesh_lighting");
    engine::renderer::begin_scene(m3DCamera, texturedLightingShader);

    // Set up some of the scene's parameters in the shader
    std::dynamic_pointer_cast<engine::gl_shader>(texturedLightingShader)->set_uniform(
        "gEyeWorldPos", m3DCamera.position());

    // Position the skybox centred on the player and render it
    glm::mat4 skyboxTransform(1.0f);
    skyboxTransform = glm::translate(skyboxTransform, m3DCamera.position());
    for (const auto& texture : mSkybox->textures()) {
        texture->bind();
    }
    engine::renderer::submit(texturedLightingShader, mSkybox, skyboxTransform);

    engine::renderer::submit(texturedLightingShader, mTerrain);

    // TREE
    glm::mat4 treeTransform(1.0f);
    treeTransform = glm::translate(treeTransform, glm::vec3(4.f, 0.5, -5.0f));
    treeTransform = glm::rotate(treeTransform, mTree->rotation_amount(), mTree->rotation_axis());
    treeTransform = glm::scale(treeTransform, mTree->scale());
    engine::renderer::submit(texturedLightingShader, treeTransform, mTree);





    // COW
    glm::mat4 cowTransform(1.0f);
    glm::vec3 p = glm::vec3(0.0f, 0.5f, 5.0f); // Position of the cow
    // cow_transform = glm::translate(cow_transform, m_cow->position());
    glm::vec3 c = m3DCamera.position();
    glm::vec3 v = c - p;
    auto angle = atan2(v.x, v.z);
    // if (angle<0) {
    //     angle *= -1;
    // }


    cowTransform = glm::translate(cowTransform, p);
    cowTransform = glm::rotate(cowTransform, angle, mCow->rotation_axis());
    cowTransform = glm::scale(cowTransform, mCow->scale());
    engine::renderer::submit(texturedLightingShader, cowTransform, mCow);

    // cow_transform = glm::translate(cow_transform, glm::vec3(0.f, 1.5f, 0.f));

    engine::renderer::submit(texturedLightingShader, cowTransform, mCow);


    if (mPickup->active()) {
        mPickup->textures().at(0)->bind();
        glm::mat4 pickupTransform(1.0f);
        pickupTransform = glm::translate(pickupTransform, mPickup->position());
        pickupTransform = glm::rotate(pickupTransform, mPickup->rotation_amount(), mPickup->rotation_axis());
        engine::renderer::submit(texturedLightingShader, mPickup->meshes().at(0), pickupTransform);
    }

    engine::renderer::end_scene();


    // Set up material shader. (does not render textures, renders materials instead)
    const auto materialShader = engine::renderer::shaders_library()->get("mesh_material");
    engine::renderer::begin_scene(m3DCamera, materialShader);

    mMaterial->submit(materialShader);
    std::dynamic_pointer_cast<engine::gl_shader>(materialShader)->set_uniform("gEyeWorldPos", m3DCamera.position());

    engine::renderer::submit(materialShader, mBall);


    // TETRAHEDRON
    mTetrahedronMaterial = engine::material::create(32.0f,
        glm::vec3(1.f, 0.5f, 0.0f),
        glm::vec3(1.f, 0.5f, 0.0f),
        glm::vec3(.5f, 0.5f, 0.0f), 0.3f );
    mTetrahedronMaterial->submit(materialShader); //reusing and overwritng i think
    engine::renderer::submit(materialShader, mTetrahedron);

    engine::renderer::end_scene();






    const auto animatedMeshShader = engine::renderer::shaders_library()->get("animated_mesh");
    engine::renderer::begin_scene(m3DCamera, animatedMeshShader);
    std::dynamic_pointer_cast<engine::gl_shader>(animatedMeshShader)->set_uniform(
        "gEyeWorldPos", m3DCamera.position());

    glm::mat4 aniTransform = glm::mat4(1.0f);

    engine::renderer::submit(animatedMeshShader, mMannequin);

    engine::renderer::end_scene();

    // Render text
    const auto textShader = engine::renderer::shaders_library()->get("text_2D");
    mTextManager->render_text(textShader, "Orange Text", 10.f, (float)engine::application::window().height() - 25.f,
                                0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
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
    if (mPrevSphereYVel< 0.f && mGameObjects.at(1)->velocity().y > 0.f)
        mAudioManager->play("bounce");
    
    mPrevSphereYVel = mGameObjects.at(1)->velocity().y;
}
