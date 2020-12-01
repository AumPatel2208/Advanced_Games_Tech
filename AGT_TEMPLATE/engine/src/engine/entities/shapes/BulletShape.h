#pragma once
#include <engine.h>

namespace engine {
    class mesh;

    /// \brief Class creating a Bullet object with a mesh of a specified size and breadth/thickness
    class BulletShape {
    public:
        /// \brief Constructor
        BulletShape(std::vector<glm::vec3> vertices);
        // engine::ref<engine::Tetrahedron> initialise(float scale);

        /// \brief Destructor
        ~BulletShape();

        /// \brief Getter methods
        std::vector<glm::vec3> vertices() const { return mVertices; }
        ref<mesh> mesh() const { return mMesh; }

        static ref<BulletShape> create(const std::vector<glm::vec3>& vertices);
        static engine::ref<engine::BulletShape> createDefaultVertices(const float& length, const float& width, const float& height);

    private:
        /// \brief Fields
        // vertices of the tetrahedron specified by a vector of positions.
        std::vector<glm::vec3> mVertices;

        ref<engine::mesh> mMesh;
    };
}


