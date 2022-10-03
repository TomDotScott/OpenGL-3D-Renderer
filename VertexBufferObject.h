#pragma once
#include <glad/glad.h>

#include "OpenGLObject.h"

struct VertexBufferObject : public OpenGLObject
{
	VertexBufferObject(const GLfloat* vertices, GLsizeiptr size);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;
};
