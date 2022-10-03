#pragma once
#include <glad/glad.h>
#include "OpenGLObject.h"

struct ElementBufferObject : public OpenGLObject
{
	ElementBufferObject(const GLuint* indices, GLsizeiptr size);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;
};
