#pragma once

#include <string>
#include <vector>

#include "Camera.h"
#include "ElementBufferObject.h"
#include "Texture.h"
#include "Vertex.h"
#include "VertexArrayObject.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, const glm::vec3& position = glm::vec3(0.f));

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);
	void Render(const Shader& shader, const Camera& camera);

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;

	VertexArrayObject m_vao;

};
