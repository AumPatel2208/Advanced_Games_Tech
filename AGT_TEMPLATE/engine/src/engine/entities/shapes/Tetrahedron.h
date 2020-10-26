#pragma once

namespace engine {
    class mesh;

    /// \brief Class creating a tetrahedron object with a mesh of a specified size
    class Tetrahedron {
    public:
        ///brief Constructor
        Tetrahedron(std::vector<glm::vec3> vertices);

        ///brief Destructor
        ~Tetrahedron();

        /// \brief Getter methods
        std::vector<glm::vec3> getVertices() const { return mVertices; }
        ref<engine::mesh> getMesh() const { return mMesh; }

        static ref<Tetrahedron> create(std::vector<glm::vec3> vertices);

    private:
        std::vector<glm::vec3> mVertices;
        std::shared_ptr<mesh> mMesh;
    };
}
