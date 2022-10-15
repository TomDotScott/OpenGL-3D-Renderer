#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.h"
#include "ElementBufferObject.h"
#include "Texture.h"
#include "Vertex.h"
#include "VertexArrayObject.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);

	void Render(const Camera& camera, const Shader& shader,  const glm::mat4& modelMatrix = glm::mat4(1.f), const glm::vec3& translation = glm::vec3(0.f), const glm::quat& rotation = glm::quat(1.f, 0.f, 0.f, 0.f), const glm::vec3& scale = glm::vec3(1.f)) const;

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

	VertexArrayObject m_vao;
};
