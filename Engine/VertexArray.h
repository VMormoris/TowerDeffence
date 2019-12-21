#pragma once
#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "GLObject.h"
#include "engpch.h"


namespace Engine {

	struct ENGINE_API Attribute {
		int size;
		unsigned int type;
		unsigned char normals;
		int stride;
		void* offset;
		//Constructor
		Attribute(int sz) {
			size = sz;
			type = GL_FLOAT;
			normals = GL_FALSE;
			stride = 0;
			offset = 0;
		}
	};

	class ENGINE_API VertexArray : public GLObject {
	private:
		unsigned int index;
	public:

		VertexArray(void);
		~VertexArray(void);

		void Bind(void) const override;

		void Unbind(void) const override;

		void Generate(void) override;
		
		void AddAttribute(const Attribute& attr);

	};
}



#endif // !VERTEX_ARRAY_H


