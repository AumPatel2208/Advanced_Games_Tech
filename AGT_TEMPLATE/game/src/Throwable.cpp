#include "Throwable.h"

#include "FriendlyNPC.h"

// Constructor
Throwable::Throwable() {
    mSize = 0.35f; // set a fixed size when first initialised
    calculateSpeed(); // calculate the speed based on the size
}

Throwable::~Throwable() {}


void Throwable::initialise() {
    // float radius = 0.5f
    // initailise the throwbale object based on the octahedron shape
    engine::ref<engine::Octahedron> octahedronShape = engine::Octahedron::createDefaultVertices(mSize);
    engine::game_object_properties octaProps;
    octaProps.position = { 0.f, 5.f, 0.f };
    octaProps.meshes = { octahedronShape->mesh() };
    octaProps.type = 0;
    octaProps.bounding_shape = glm::vec3(mSize);
    octaProps.restitution = 0.f;
    octaProps.mass = 1.f;
    octaProps.textures = { engine::texture_2d::create("assets/textures/bullet.png", false) };
    mObject = engine::game_object::create(octaProps);
    mObject->setName("throwable"); // set the name to a throwable

}

// render the object
void Throwable::onRender(const std::shared_ptr<engine::shader>& texturedLightingShader) const {
    engine::renderer::submit(texturedLightingShader, mObject);
}

// render the up to interact with the player.
void Throwable::renderPickupHUD(engine::ref<engine::text_manager>& textManager) {
    if (toRenderPickup) {
        if (isActive) {
            const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
            textManager->render_text(text_shader, "(Right Click) Throw", 600.f, 100.f, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
            textManager->render_text(text_shader, "Y/U size", 700.f, 175.f, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
        }
        else {
            const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
            textManager->render_text(text_shader, "(Right Click) Pickup", 600.f, 100.f, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
        }
    }
}

void Throwable::onUpdate(const engine::timestep& timestep, Player& player) {

    // calculate distance from the player
    auto d = mObject->position() - player.object()->position();
    auto distanceFromPlayer = glm::length(d);
    if (isInRange(distanceFromPlayer)) {
        toRenderPickup = true;
    }else {
        toRenderPickup = false;
    }

    //Turn the object with the player
    if (isActive) {
        // turn Apply angular velocity
        // move the object with the player
        pickUpObject(player.object()->position());
        player.decreaseStamina(0.1f);
    }

    // timer to provide a space between interacting with the objects with button inputs (required to avoid the issue of multiple click in a short time frame)
    if(interactTimer>0) {
        interactTimer -= timestep;
    }

    // timer used to tell that the object is still in the air being thrown.
    if(throwTimer>0) {
        throwTimer-= timestep;
    }

    if (interactTimer <= 0) {
        if (engine::input::mouse_button_pressed(1)) {
            if (isActive) {
                throwObject(player.object()->forward());
                isActive = false;
            }else {
                if(isInRange(distanceFromPlayer)) {
                    isActive = true;
                    pickUpObject(player.object()->position());
                }
            }
            interactTimer = 0.5f;
        }
        else if (engine::input::key_pressed(engine::key_codes::KEY_U)) {
            // scale the object up
            if (isActive) {
                mSize += 0.1f;
                mObject->set_scale(glm::vec3(mSize));
                calculateSpeed();
                mObject->set_bounding_shape(mObject->scale());
                interactTimer = 0.5f;
            }
        }
        else if (engine::input::key_pressed(engine::key_codes::KEY_Y)) {
            // scale the object down 
            if (isActive) {
                mSize -= 0.1f;
                mObject->set_scale(glm::vec3(mSize));
                calculateSpeed();
                mObject->set_bounding_shape(mObject->scale());
                interactTimer = 0.5f;
            }
        }
    }

}

void Throwable::throwObject(const glm::vec3& playerForward) {
    // use physics and direction to send the object flying (Enable gravity again)
    mObject->set_mass(0);
    mObject->set_velocity(glm::vec3(mThrowSpeed*4)*glm::normalize(playerForward));
    throwTimer = 2.f;
}

void Throwable::pickUpObject(const glm::vec3& playerPosition) {
    // disable gravity and set the position in the  correct position
    mObject->set_position(glm::vec3(playerPosition.x, playerPosition.y+1.f, playerPosition.z));
}

bool Throwable::isInRange(const float distanceFromPlayer) {
    // will return true if the player is in range of the object to pick it up
    return (distanceFromPlayer <= 1.f);
}

void Throwable::calculateSpeed() {
    // calculate the velocity at which the object will be thrown
    mThrowSpeed = 1 / mSize;
}
