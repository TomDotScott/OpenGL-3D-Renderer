#pragma once
#include <vector>
#include <glad/glad.h>

#include "OpenGLObject.h"

struct Vertex;

struct VertexBufferObject : public OpenGLObject
{
	VertexBufferObject(const std::vector<Vertex>& vertices);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;
};
