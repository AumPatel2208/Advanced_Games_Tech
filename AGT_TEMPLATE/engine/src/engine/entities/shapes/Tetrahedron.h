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

        void initialise(glm::vec3 p1, glm::vec3 p2, double r1, double r2, int m, double theta1, double theta2);

        /// \brief Destructor
        ~Tetrahedron();

        /// \brief Getter methods
        std::vector<glm::vec3> vertices() const { return m_vertices; }
        ref<engine::mesh> mesh() const { return m_mesh; }

        static ref<Tetrahedron> create(std::vector<glm::vec3> vertices);
        static engine::ref<engine::Tetrahedron> createDefaultVertices(const float& scale);
    private:
        /// \brief Fields
        // vertices of the tetrahedron specified by a vector of positions.
        std::vector<glm::vec3> m_vertices;
        std::list<renderVertex> mVertices = {};

        ref<engine::mesh> m_mesh;
    };
}
