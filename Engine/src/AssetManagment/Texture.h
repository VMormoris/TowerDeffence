#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "../Core/utils.h"
#include "../OpenGL/GLObject.h"

namespace Engine {

	class ENGINE_API Texture : public GLObject {
	protected:
		/**
		* filename of the texture
		*/
		std::string m_filename;
		/**
		* Mipmaps flag
		*/
		bool m_hasMipmaps;
	public:

		Texture(const char* filename, bool hasMipmaps = false);
		~Texture(void);
		
		void Bind()const;
		void Unbind()const;

		unsigned int GetHandle(void);
		std::string GetName(void);
		bool GetMipmap(void);

		void SetTextureImage(int internal_format, int WIDTH, int HEIGHT, unsigned int format, unsigned int type, const void* pixels);
		
		void LoadImage(void);
		
		template<typename T>
		void setParameters(T wrapfunc, T filter) { static_assert(false); }

		template<>
		void setParameters<float>(float wrapfunc, float filter){
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapfunc));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapfunc));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
		}

		template<>
		void setParameters<int>(int wrapfunc, int filter) {
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapfunc));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapfunc));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
		}

		void Delete(void);
		void Generate(void);

	};

}

#endif // !TEXTURE_H
