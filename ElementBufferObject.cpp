#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject(std::vector<GLuint>& indices) :
	OpenGLObject(0)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void ElementBufferObject::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void ElementBufferObject::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::Delete() const
{
	glDeleteBuffers(1, &m_ID);
}