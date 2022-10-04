#pragma once
#include <glad/glad.h>
#include "OpenGLObject.h"
#include "VertexBufferObject.h"

struct VertexArrayObject : OpenGLObject
{
	VertexArrayObject();

	void LinkAttrib(const VertexBufferObject& VBO, GLuint layout, GLint numComponents, GLenum type, GLsizeiptr stride, const void* offset);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;
};
