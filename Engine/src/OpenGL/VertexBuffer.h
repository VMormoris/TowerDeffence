#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "GLObject.h"

namespace Engine {

	class ENGINE_API VertexBuffer: public GLObject {
	public:

		VertexBuffer(void);
		~VertexBuffer(void);
		
		void Bind(void) const override;

		void Unbind(void) const override;

		void Generate(void) override;
	
		void FillBuffer(const void* data, unsigned int size);

	};

}

#endif // !VERTEX_BUFFER_H
