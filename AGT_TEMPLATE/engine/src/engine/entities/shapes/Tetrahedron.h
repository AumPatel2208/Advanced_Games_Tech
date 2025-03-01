#pragma once

namespace engine {
    class mesh;

    struct renderVertex {
        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    /// \brief Class creating a tetrahedron object with a mesh of a specified size
    class Tetrahedron {
    public:
        /// \brief Constructor
        Tetrahedron(std::vector<glm::vec3> vertices);
        // engine::ref<engine::Tetrahedron> initialise(float scale);

        /// \brief Destructor
        ~Tetrahedron();

        /// \brief Getter methods
        std::vector<glm::vec3> vertices() const { return mVertices; }
        ref<mesh> mesh() const { return mMesh; }

        static ref<Tetrahedron> create(const std::vector<glm::vec3>& vertices);
        static engine::ref<engine::Tetrahedron> createDefaultVertices(const float& scale);
    private:
        /// \brief Fields
        // vertices of the tetrahedron specified by a vector of positions.
        std::vector<glm::vec3> mVertices;
        // std::list<renderVertex> mVertices = {};

        ref<engine::mesh> mMesh;
    };
}
