#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject() : OpenGLObject(0)
{
	glGenVertexArrays(1, &m_ID);
}

void VertexArrayObject::LinkVertexBufferObject(const VertexBufferObject& VBO, const GLuint layout)
{
	VBO.Bind();

	glVertexAttribPointer(
		layout,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(layout);

	VBO.Unbind();
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::Delete() const
{
	glDeleteVertexArrays(1, &m_ID);
}
