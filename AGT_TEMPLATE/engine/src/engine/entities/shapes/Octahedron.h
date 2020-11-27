#pragma once

namespace engine {
    class mesh;

    /// \brief Class creating a tetrahedron object with a mesh of a specified size
    class Octahedron {
    public:
        /// \brief Constructor
        Octahedron(std::vector<glm::vec3> vertices);
        // engine::ref<engine::Tetrahedron> initialise(float scale);

        /// \brief Destructor
        ~Octahedron();

        /// \brief Getter methods
        std::vector<glm::vec3> vertices() const { return mVertices; }
        ref<mesh> mesh() const { return mMesh; }

        static ref<Octahedron> create(const std::vector<glm::vec3>& vertices);
        static engine::ref<engine::Octahedron> createDefaultVertices(const float& scale);

    private:
        /// \brief Fields
        // vertices of the tetrahedron specified by a vector of positions.
        std::vector<glm::vec3> mVertices;

        ref<engine::mesh> mMesh;
    };
}

