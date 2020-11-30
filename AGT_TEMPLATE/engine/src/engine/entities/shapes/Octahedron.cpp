#include "pch.h"
#include "Octahedron.h"


engine::Octahedron::Octahedron(const std::vector<glm::vec3> vertices) : mVertices(vertices) {
    std::vector<glm::vec3> normals;

    // top half
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(3)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(4)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(1)));

    // bottom half
    normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(3)));
    normals.push_back(glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) - vertices.at(4)));
    normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(1)));

    std::vector<mesh::vertex> octaVertices
    {
        //position          Normal          tex Coord

        //
        {vertices.at(0), normals.at(0), {0.f, 0.f}},
        {vertices.at(1), normals.at(0), {1.f, 0.f}},
        {vertices.at(2), normals.at(0), {0.5f, 1.f}},

        {vertices.at(0), normals.at(1), {0.f, 0.f}},
        {vertices.at(2), normals.at(1), {1.f, 0.f}},
        {vertices.at(3), normals.at(1), {0.5f, 1.f}},

        {vertices.at(0), normals.at(2), {0.f, 0.f}},
        {vertices.at(3), normals.at(2), {1.f, 0.f}},
        {vertices.at(4), normals.at(2), {0.5f, 1.f}},

        {vertices.at(0), normals.at(3), {0.f, 0.f}},
        {vertices.at(4), normals.at(3), {1.f, 0.f}},
        {vertices.at(1), normals.at(3), {0.5f, 1.f}},

        {vertices.at(2), normals.at(4), {0.f, 0.f}},
        {vertices.at(1), normals.at(4), {1.f, 0.f}},
        {vertices.at(5), normals.at(4), {0.5f, 1.f}},

        {vertices.at(3), normals.at(5), {0.f, 0.f}},
        {vertices.at(2), normals.at(5), {1.f, 0.f}},
        {vertices.at(5), normals.at(5), {0.5f, 1.f}},

        {vertices.at(4), normals.at(6), {0.f, 0.f}},
        {vertices.at(3), normals.at(6), {1.f, 0.f}},
        {vertices.at(5), normals.at(6), {0.5f, 1.f}},


        {vertices.at(1), normals.at(7), {0.f, 0.f}},
        {vertices.at(4), normals.at(7), {1.f, 0.f}},
        {vertices.at(5), normals.at(7), {0.5f, 1.f}},

    };

    const std::vector<uint32_t> octaIndices
    {
        0, 1, 2, //front
        3, 4, 5, //left
        6, 7, 8, //Right
        9, 10, 11, //Bottom
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23
    };


    mMesh = mesh::create(octaVertices, octaIndices);
}

engine::Octahedron::~Octahedron() {}

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
    octaVertices.emplace_back(0.f, -1.f * scale, 0.f); // 5 // Bottom vertex

    return create(octaVertices);

}
