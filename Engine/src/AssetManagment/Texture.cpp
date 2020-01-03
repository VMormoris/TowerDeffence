#include "../../engpch.h"
#include "Texture.h"


namespace Engine {

	Texture::Texture(const char* filename, bool hasMipmaps) {
		m_filename=std::string(filename);
		m_hasMipmaps = hasMipmaps;
		m_ID = 0;
	}

	Texture::~Texture(void) { Delete();	}

	void Texture::Delete(void) {
		if (m_ID != 0) { GLCall(glDeleteTextures(1, &m_ID)); m_ID = 0; }
	}

	void Texture::SetTextureImage(int internal_format, int WIDTH, int HEIGHT, unsigned int format, unsigned int type, const void* pixels) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, WIDTH, HEIGHT, 0, format, type, pixels));
	}

	void Texture::LoadImage(void) {

		//Load texture
		SDL_Surface* surface = IMG_Load(m_filename.c_str());

		if (surface == NULL)
		{
			printf("Could not Load texture %s\n", m_filename.c_str());
			printf("SDL load Error %s\n", SDL_GetError());
			return; // error
		}


		Generate();
		Bind();
		unsigned int texture_format;
		int numofcolors = surface->format->BytesPerPixel;

		switch (surface->format->BytesPerPixel)
		{
		case 4: // contains alpha channel
			if (surface->format->Rmask == 0x000000ff) texture_format = GL_RGBA;
			else texture_format = GL_BGRA;
			numofcolors = GL_RGBA;
			break;
		case 3: // no alpha channel
			if (surface->format->Rmask == 0x000000ff) texture_format = GL_RGB;
			else texture_format = GL_BGR;
			numofcolors = GL_RGB;
			break;
		default:
			printf("Error in number of colors at %s\n", m_filename.c_str());
		}

		unsigned char* data = new unsigned char[surface->w * surface->h * surface->format->BytesPerPixel];
		if (data == NULL) { return; }//Error while Allocating memory

		// flip image
		for (int y = 0; y < surface->h; y++)
			memcpy(&data[(surface->h - y - 1) * surface->w * surface->format->BytesPerPixel], &static_cast<unsigned char*>(surface->pixels)[y * surface->w * surface->format->BytesPerPixel], surface->w* surface->format->BytesPerPixel * sizeof(unsigned char));

		SDL_LockSurface(surface);
		SetTextureImage(numofcolors, surface->w, surface->h, texture_format, GL_UNSIGNED_BYTE, data);
		SDL_UnlockSurface(surface);
		delete[] data;

		setParameters<float>(GL_REPEAT, GL_LINEAR);
		if (m_hasMipmaps)
		{
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		}
		if (surface) SDL_FreeSurface(surface);
		Unbind();
	}


	void Texture::Generate(void) { GLCall(glGenTextures(1, &m_ID)); }
	void Texture::Bind() const { GLCall(glBindTexture(GL_TEXTURE_2D, m_ID)); }
	void Texture::Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
	unsigned int Texture::GetHandle(void) { return m_ID; }
	std::string Texture::GetName(void) { return m_filename; }
	bool Texture::GetMipmap(void) { return m_hasMipmaps; }
}