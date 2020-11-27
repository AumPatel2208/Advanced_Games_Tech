#include "pch.h"
#include "Tetrahedron.h"
#include <engine.h>		

engine::Tetrahedron::Tetrahedron(std::vector<glm::vec3> vertices) : m_vertices(vertices) {
	std::vector<glm::vec3> normals;

    // front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	// left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	// right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(3)));
	// bottom normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(3)));

	


	std::vector<mesh::vertex> primitiveVerticies
	{
		//front
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f, 1.f } },

		//left
		{ vertices.at(0),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f, 1.f } },

		//right
		{ vertices.at(3),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(2),		{ 0.5f, 1.f } },

		//bottom
		{ vertices.at(3),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(3),		{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> primitiveShapeIndicies
	{
		0,  1,  2,	//front
		3,  4,  5,	//left
		6,	7,	8,	//Right
		9,	10,	11	//Bottom
	};


	m_mesh = engine::mesh::create(primitiveVerticies, primitiveShapeIndicies);
}


engine::Tetrahedron::~Tetrahedron() {}

engine::ref<engine::Tetrahedron> engine::Tetrahedron::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::Tetrahedron>(vertices);
}

engine::ref<engine::Tetrahedron> engine::Tetrahedron::createDefaultVertices(const float& scale) {
    std::vector<glm::vec3> primitiveVerticies;
        primitiveVerticies.push_back(glm::vec3(0.f, 1.f * scale, 0.f)); //0 //top vertex
        primitiveVerticies.push_back(glm::vec3(0.f, 0.f, 1.f * scale)); //1
        primitiveVerticies.push_back(glm::vec3(-1.f * scale, 0.f, -1.f * scale)); //2
        primitiveVerticies.push_back(glm::vec3(1.f * scale, 0.f, -1.f * scale)); //3


    return Tetrahedron::create(primitiveVerticies);
}
