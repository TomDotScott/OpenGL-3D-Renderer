#pragma once
#include <vector>
#include <glad/glad.h>
#include "OpenGLObject.h"

struct ElementBufferObject : public OpenGLObject
{
	ElementBufferObject(std::vector<GLuint>& indices);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;
};
