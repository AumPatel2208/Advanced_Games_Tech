#include "pch.h"
#include "Octahedron.h"
#include <engine.h>


engine::Octahedron::Octahedron(const std::vector<glm::vec3> vertices) :mVertices(vertices){
    std::vector<glm::vec3> normals;

    
    std::vector < mesh::vertex > octaVertices
    {
        //position          Normal          tex Coord

        //
        {vertices.at(0), normals.at(0),{0.f,0.f}},
        {vertices.at(1), normals.at(0),{0.f,0.f}},
        {vertices.at(2), normals.at(0),{0.f,0.f}},

        {vertices.at(0), normals.at(0),{0.f,0.f}},
        {vertices.at(2), normals.at(0),{0.f,0.f}},
        {vertices.at(3), normals.at(0),{0.f,0.f}},

        {vertices.at(0), normals.at(0),{0.f,0.f}},
        {vertices.at(3), normals.at(0),{0.f,0.f}},
        {vertices.at(4), normals.at(0),{0.f,0.f}},
        

        {vertices.at(0), normals.at(0),{0.f,0.f}},
        {vertices.at(4), normals.at(0),{0.f,0.f}},
        {vertices.at(1), normals.at(0),{0.f,0.f}},

        {vertices.at(5), normals.at(0),{0.f,0.f}},
        {vertices.at(1), normals.at(0),{0.f,0.f}},
        {vertices.at(2), normals.at(0),{0.f,0.f}},

        {vertices.at(5), normals.at(0),{0.f,0.f}},
        {vertices.at(2), normals.at(0),{0.f,0.f}},
        {vertices.at(3), normals.at(0),{0.f,0.f}},

        {vertices.at(5), normals.at(0),{0.f,0.f}},
        {vertices.at(3), normals.at(0),{0.f,0.f}},
        {vertices.at(4), normals.at(0),{0.f,0.f}},
        

        {vertices.at(5), normals.at(0),{0.f,0.f}},
        {vertices.at(4), normals.at(0),{0.f,0.f}},
        {vertices.at(1), normals.at(0),{0.f,0.f}},

    };

}

std::shared_ptr<engine::Octahedron> engine::Octahedron::create(const std::vector<glm::vec3>& vertices) {
    return std::make_shared<engine::Octahedron>(vertices);
}
engine::ref<engine::Octahedron> engine::Octahedron::createDefaultVertices(const float& scale) {
    std::vector<glm::vec3> octaVertices;

    octaVertices.emplace_back(0.f, 1.f * scale, 0.f); // 0 // top vertex
    octaVertices.emplace_back(1.f * scale, 0.f, 1.f * scale); // 1
    octaVertices.emplace_back(1.f * scale, 0.f, -1.f * scale); // 2
    octaVertices.emplace_back(-1.f * scale, 0.f, -1.f * scale); // 3
    octaVertices.emplace_back(-1.f * scale, 0.f, 1.f * scale); // 4
    octaVertices.emplace_back(0.f, -1.f*scale, 0.f); // 5 // Bottom vertex

    return Octahedron::create(octaVertices);

}
