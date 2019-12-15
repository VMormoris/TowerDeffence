#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Texture.h"
#include <unordered_map>

namespace Engine {
	

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
		static int findTexture(const char* filename, bool Mipmaps);
		/**
		* Container for all the textures in the current instance of TextureManager
		*/
		static std::vector<Texture*> textures;
	public:
		/**
		* Delete all textures
		*/
		static void Clear(void);
		/**
		* Request texture handle (aka ID)
		* @param filename Filename associated with the texture
		* @param hasMipmaps Mipmpaps flag of the texture
		* @return A handle (ID) to the requested texture, or 0 if the texture can't be founded and loaded
		*/
		static unsigned int RequestTexture(const char* filename, bool hasMipmaps = false);
	};

}


#endif // !TEXTURE_MANAGER_H
