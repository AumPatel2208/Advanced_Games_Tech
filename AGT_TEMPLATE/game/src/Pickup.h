#pragma once
#include <engine.h>

class pickup: public engine::game_object {

public:
	pickup(const engine::game_object_properties props); // Constructor
	~pickup(); // Destructor
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_isActive; }

	static engine::ref<pickup> create(const engine::game_object_properties& props); // used to create the class in the engine after passing in props?



private:
	bool m_isActive; // indicates if the pickup is active



};
