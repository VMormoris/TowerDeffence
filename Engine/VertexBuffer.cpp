#include "engpch.h"
#include "VertexBuffer.h"
#include "utils.h"



namespace Engine {

	VertexBuffer::VertexBuffer(void) { m_ID = 0; }

	VertexBuffer::~VertexBuffer(void){ if (m_ID != 0) { GLCall(glDeleteBuffers(1, &m_ID)); } }

	void VertexBuffer::Generate(void) { GLCall(glGenBuffers(1, &m_ID)); }

	void VertexBuffer::Bind(void) const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID)); }

	void VertexBuffer::Unbind(void) const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

	void VertexBuffer::FillBuffer(const void* data, unsigned int size) {
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

}