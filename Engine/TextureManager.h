#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "utils.h"
#include <string>
#include <unordered_map>

namespace Engine {
	/**
	* Struct that represents a texture
	*/
	struct ENGINE_API Texture {
		/**
		* ID of a binded texture
		*/
		unsigned int m_texture_ID;
		/**
		* filename of the texture
		*/
		std::string m_filename;
		/**
		* Mipmaps flag
		*/
		bool m_hasMipmaps;
	};

	/**
	* Class that helpful for handling multiple textures
	*/
	class ENGINE_API TextureManager {
	protected:
		/**
		* Find the texture with the given filename and Mipmaps
		* @param filename Filename of the texture
		* @param Mipmaps Mipmaps flag
		* @return The index of the texture if already exists, -1 otherwise
		*/
		int findTexture(const char* filename, bool Mipmaps);
		/**
		* Container for all the textures in the current instance of TextureManager
		*/
		std::vector<Texture> textures;
	public:
		TextureManager();
		~TextureManager();
		/**
		* Delete all textures
		*/
		void Clear(void);
		/**
		* Request texture handle (aka ID)
		* @param filename Filename associated with the texture
		* @param hasMipmaps Mipmpaps flag of the texture
		* @return A handle (ID) to the requested texture, or 0 if the texture can't be founded and loaded
		*/
		unsigned int RequestTexture(const char* filename, bool hasMipmaps = false);
	};

}


#endif // !TEXTURE_MANAGER_H
