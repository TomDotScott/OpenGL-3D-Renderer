#pragma once
#include <glad/glad.h>
#include "OpenGLObject.h"
#include "VertexBufferObject.h"

struct VertexArrayObject : OpenGLObject
{
	VertexArrayObject();

	void LinkVertexBufferObject(const VertexBufferObject& VBO, GLuint layout);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;
};
