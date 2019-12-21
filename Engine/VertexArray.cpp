#include "engpch.h"
#include "VertexArray.h"
#include "utils.h"



namespace Engine{
	VertexArray::VertexArray(void) {
		m_ID = 0;
		index = 0;
	}

	VertexArray::~VertexArray(void) {
		if (m_ID != 0) { GLCall(glDeleteVertexArrays(1, &m_ID)); }
	}

	void VertexArray::Bind(void) const {
		GLCall(glBindVertexArray(m_ID));
	}

	void VertexArray::Unbind(void) const {
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::Generate(void) {
		GLCall(glGenVertexArrays(1, &m_ID));
	}

	void VertexArray::AddAttribute(const Attribute& attr) {
		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(
			index,				// attribute index
			attr.size,              // number of elements per vertex, here (x,y,z)
			attr.type,       // the type of each element
			attr.normals,       // take our values as-is
			attr.stride,		        // no extra data between each position
			attr.offset				// pointer to the C array or an offset to our buffer
		));
		index++;
	}
}