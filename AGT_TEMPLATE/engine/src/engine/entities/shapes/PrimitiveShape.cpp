#include "pch.h"
#include "PrimitiveShape.h"
#include <engine.h>		

engine::PrimitiveShape::PrimitiveShape(std::vector<glm::vec3> vertices) : m_vertices(vertices) {
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

	//
	// auto p1 = glm::vec3(0.f,0.f,0.f);
	// auto p2 = glm::vec3(0.f,3.f,0.f);
	// auto r = (double)2.0;
	// int m = 4;
	// double theta1 =30.;
	// // double theta1 =double( glm::radians(30.f));
	// double theta2 = 50.;
	// // double theta2 = double(glm::radians(50.f));
	//
	// initialise(p1, p2, r, r, m, theta1, theta2);
	//
	// std::vector<mesh::vertex> cylinderVerticies;
	//
	// for (auto& vertex : mVertices) {
	// 	mesh::vertex meshVertex = { vertex.vertex,vertex.normal, {vertex.texCoord.x, vertex.texCoord.y} };
	// 	cylinderVerticies.push_back(meshVertex);
	// }
	//
	//
	// std::vector<uint32_t> cylinderIndicies;
	// for (int i = 0 ; i<mVertices.size(); ++i) {
	// 	cylinderIndicies.push_back(i);
	// }







	m_mesh = engine::mesh::create(primitiveVerticies, primitiveShapeIndicies);
	// m_mesh = engine::mesh::create(cylinderVerticies,cylinderIndicies);
}


/*
   Create a cone/cylinder uncapped between end points p1, p2
   radius r1, r2, and precision m
   Create the cylinder between theta1 and theta2 in radians
*/
void engine::PrimitiveShape::initialise(glm::vec3 p1, glm::vec3 p2, double r1, double r2, int m,double theta1, double theta2) {
	
		int i, j;
		double theta;
		glm::vec3 n, p, q, perp;

		/* Normal pointing from p1 to p2 */
		n.x = p1.x - p2.x;
		n.y = p1.y - p2.y;
		n.z = p1.z - p2.z;

		/*
		   Create two perpendicular vectors perp and q
		   on the plane of the disk
		*/
		perp = n;
		if (n.x == 0 && n.z == 0)
			perp.x += 1;
		else
			perp.y += 1;


		// CROSSPROD(perp, n, q);
		q = glm::cross(perp, n);


		// CROSSPROD(n, q, perp);
		perp = glm::cross(n, q);
		// Normalise(&perp);
		// Normalise(&q);
		perp = glm::normalize(perp);
		q = glm::normalize(q);

        
		// glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= m; i++) {
			theta = theta1 + i * (theta2 - theta1) / m;

			n.x = cos(theta) * perp.x + sin(theta) * q.x;
			n.y = cos(theta) * perp.y + sin(theta) * q.y;
			n.z = cos(theta) * perp.z + sin(theta) * q.z;
			// Normalise(&n);
			n = glm::normalize(n);

			p.x = p2.x + r2 * n.x;
			p.y = p2.y + r2 * n.y;
			p.z = p2.z + r2 * n.z;

			// glNormal3f(n.x, n.y, n.z);
			// glTexCoord2f(i / (double)m, 1.0);
			// glVertex3f(p.x, p.y, p.z);

			renderVertex vertex;
			vertex.normal = n;
			vertex.texCoord = glm::vec2(i / (double)m, 1.0f);
			vertex.vertex = p;
			mVertices.push_back(vertex);


			p.x = p1.x + r1 * n.x;
			p.y = p1.y + r1 * n.y;
			p.z = p1.z + r1 * n.z;
			// glNormal3f(n.x, n.y, n.z);
			// glTexCoord2f(i / (double)m, 0.0);
			// glVertex3f(p.x, p.y, p.z);

			vertex.normal = n;
			vertex.texCoord = glm::vec2(i / (double)m, 1.0f);
			vertex.vertex = p;
			mVertices.push_back(vertex);
		}
		// glEnd();


}

engine::PrimitiveShape::~PrimitiveShape() {}

engine::ref<engine::PrimitiveShape> engine::PrimitiveShape::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::PrimitiveShape>(vertices);
}
