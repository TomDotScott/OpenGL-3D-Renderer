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
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);

	void Render(const Shader& shader, const Camera& camera);

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

	VertexArrayObject m_vao;

};
