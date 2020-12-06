#include "pch.h"
#include "BulletShape.h"


engine::BulletShape::BulletShape(std::vector<glm::vec3> vertices) {
    std::vector<glm::vec3> normals;

    //normals for the different sides
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(4)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(3)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(4)));
    normals.push_back(glm::cross(vertices.at(2) - vertices.at(4), vertices.at(2) - vertices.at(8)));
    normals.push_back(glm::cross(vertices.at(2) - vertices.at(6), vertices.at(2) - vertices.at(8)));
    normals.push_back(glm::cross(vertices.at(2) - vertices.at(5), vertices.at(2) - vertices.at(6)));
    normals.push_back(glm::cross(vertices.at(2) - vertices.at(5), vertices.at(2) - vertices.at(1)));
    normals.push_back(glm::cross(vertices.at(1) - vertices.at(3), vertices.at(1) - vertices.at(7)));
    normals.push_back(glm::cross(vertices.at(1) - vertices.at(5), vertices.at(1) - vertices.at(7)));
    normals.push_back(glm::cross(vertices.at(4) - vertices.at(7), vertices.at(4) - vertices.at(8)));
    normals.push_back(glm::cross(vertices.at(4) - vertices.at(3), vertices.at(4) - vertices.at(7)));
    normals.push_back(glm::cross(vertices.at(7) - vertices.at(8), vertices.at(7) - vertices.at(6)));
    normals.push_back(glm::cross(vertices.at(7) - vertices.at(5), vertices.at(7) - vertices.at(6)));

    // create the triangles for the shape
    std::vector<engine::mesh::vertex> bulletShapeVertices
    {
        //position          Normal          tex Coord

        // top front
        {vertices.at(0), normals.at(0), {0.f, 0.f}},
        {vertices.at(2), normals.at(0), {1.f, 0.f}},
        {vertices.at(4), normals.at(0), {0.5f, 1.f}},

        // top right
        {vertices.at(0), normals.at(0), {0.f, 0.f}},
        {vertices.at(1), normals.at(0), {1.f, 0.f}},
        {vertices.at(2), normals.at(0), {0.5f, 1.f}},

        // top back
        {vertices.at(0), normals.at(0), {0.f, 0.f}},
        {vertices.at(3), normals.at(0), {0.5f, 1.f}},
        {vertices.at(1), normals.at(0), {1.f, 0.f}},

        // top left
        {vertices.at(0), normals.at(0), {0.f, 0.f}},
        {vertices.at(4), normals.at(0), {0.5f, 1.f}},
        {vertices.at(3), normals.at(0), {1.f, 0.f}},

        // front 1
        {vertices.at(2), normals.at(0), {0.f, 0.f}},
        {vertices.at(8), normals.at(0), {1.f, 1.f}},
        {vertices.at(4), normals.at(0), {1.f, 0.f}},
        // front 2
        {vertices.at(2), normals.at(0), {0.f, 0.f}},
        {vertices.at(6), normals.at(0), {1.f, 0.f}},
        {vertices.at(8), normals.at(0), {1.f, 1.f}},

        // right 1
        {vertices.at(2), normals.at(0), {0.f, 0.f}},
        {vertices.at(5), normals.at(0), {1.f, 0.f}},
        {vertices.at(6), normals.at(0), {1.f, 1.f}},
        // right 2
        {vertices.at(2), normals.at(0), {0.f, 0.f}},
        {vertices.at(1), normals.at(0), {1.f, 1.f}},
        {vertices.at(5), normals.at(0), {1.f, 0.f}},

        // back 1
        {vertices.at(1), normals.at(0), {0.f, 0.f}},
        {vertices.at(3), normals.at(0), {1.f, 0.f}},
        {vertices.at(7), normals.at(0), {1.f, 1.f}},
        // back 2
        {vertices.at(1), normals.at(0), {0.f, 0.f}},
        {vertices.at(7), normals.at(0), {1.f, 1.f}},
        {vertices.at(5), normals.at(0), {1.f, 0.f}},

        // left 1
        {vertices.at(4), normals.at(0), {0.f, 0.f}},
        {vertices.at(8), normals.at(0), {1.f, 1.f}},
        {vertices.at(7), normals.at(0), {1.f, 0.f}},
        // left 2
        {vertices.at(4), normals.at(0), {0.f, 0.f}},
        {vertices.at(7), normals.at(0), {1.f, 1.f}},
        {vertices.at(3), normals.at(0), {1.f, 0.f}},

        // bottom 1
        {vertices.at(7), normals.at(0), {0.f, 0.f}},
        {vertices.at(8), normals.at(0), {1.f, 0.f}},
        {vertices.at(6), normals.at(0), {1.f, 1.f}},
        // bottom 2
        {vertices.at(7), normals.at(0), {0.f, 0.f}},
        {vertices.at(6), normals.at(0), {1.f, 1.f}},
        {vertices.at(5), normals.at(0), {1.f, 0.f}},


    };


    const std::vector<uint32_t> bulletShapeIndices
    {
        0, 1, 2, 
        3, 4, 5, 
        6, 7, 8, 
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35,
        36, 37, 38,
        39, 40, 41
    };


    mMesh = mesh::create(bulletShapeVertices, bulletShapeIndices);

}


engine::BulletShape::~BulletShape() {}

std::shared_ptr<engine::BulletShape> engine::BulletShape::create(const std::vector<glm::vec3>& vertices) {
    return std::make_shared<engine::BulletShape>(vertices);
}

// used to create a shape by providing length, width and height through using a default set of vertices
engine::ref<engine::BulletShape> engine::BulletShape::createDefaultVertices(const float& length, const float& width, const float& height) {
    std::vector<glm::vec3> bulletVertices;

    bulletVertices.emplace_back(0.f, height / 2, 2.f * length); // tip
    // right of base top
    bulletVertices.emplace_back(1 * width, 0.f, 1 * length);
    bulletVertices.emplace_back(1 * width, height, 1 * length);
    //left of base top
    bulletVertices.emplace_back(-1.f* width, 0.f, 1.f*length);
    bulletVertices.emplace_back(-1.f * width, height, 1.f * length);
    //right base
    bulletVertices.emplace_back(1.f * width, 0.f, -1.f * length);
    bulletVertices.emplace_back(1.f * width, height, -1.f * length);
    //left base
    bulletVertices.emplace_back(-1.f * width, 0.f, -1.f * length);
    bulletVertices.emplace_back(-1.f * width, height, -1.f * length);

    return create(bulletVertices);
}
