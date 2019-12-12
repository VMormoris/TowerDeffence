#include "TextureManager.h"
#include "SDL_image.h"

namespace Engine {

	TextureManager::TextureManager(void) {}

	TextureManager::~TextureManager(void) { Clear(); }

	void TextureManager::Clear(void) {
		for (Texture texture : textures) { GLCall(glDeleteTextures(1, &texture.m_texture_ID)); }
		textures.clear();
	}

	int TextureManager::findTexture(const char* filename, bool Mipmaps) {
		for (int i = 0; i < textures.size(); i++) {
			Texture texture = textures[i];
			if (texture.m_filename.compare(filename) == 0 && texture.m_hasMipmaps == Mipmaps) return i;
		}
		return -1;
	}

	unsigned int TextureManager::RequestTexture(const char* filename, bool hasMipmaps) {

		//Check if already exists
		int index = findTexture(filename, hasMipmaps);
		if (index != -1) return textures[index].m_texture_ID;


		//Load texture
		SDL_Surface* surface = IMG_Load(filename);

		if (surface == NULL)
		{
			printf("Could not Load texture %s\n", filename);
			printf("SDL load Error %s\n", SDL_GetError());
			return 0; // error
		}

		Texture texture;
		texture.m_filename = filename;
		texture.m_hasMipmaps = hasMipmaps;
		GLenum texture_format;
		int numofcolors;

		GLCall(glGenTextures(1, &texture.m_texture_ID));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture.m_texture_ID));
		numofcolors = surface->format->BytesPerPixel;

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
			printf("Error in number of colors at %s\n", filename);
		}

		unsigned char* data = new unsigned char[surface->w * surface->h * surface->format->BytesPerPixel];
		if (data == NULL) { return 0; }//Error while Allocating memory

		// flip image
		for (int y = 0; y < surface->h; y++)
			memcpy(&data[(surface->h - y - 1) * surface->w * surface->format->BytesPerPixel], &static_cast<unsigned char*>(surface->pixels)[y * surface->w * surface->format->BytesPerPixel], surface->w* surface->format->BytesPerPixel * sizeof(unsigned char));

		SDL_LockSurface(surface);
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, numofcolors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, data));
		SDL_UnlockSurface(surface);
		delete[] data;

		GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		if (hasMipmaps)
		{
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		}
		if (surface) SDL_FreeSurface(surface);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind the texture

		// save the texture
		textures.push_back(texture);
		return texture.m_texture_ID;

	}

}

