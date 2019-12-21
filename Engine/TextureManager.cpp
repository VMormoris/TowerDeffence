#include "engpch.h"
#include "TextureManager.h"

namespace Engine {

	std::vector<Texture*> TextureManager::textures;
	
	void TextureManager::Clear(void) {
		size_t size = textures.size();
		for (size_t i = 0; i < size;i++) {
			Texture* texture = textures[i];
			delete texture;
		}
		textures.clear();
	}

	int TextureManager::findTexture(const char* filename, bool Mipmaps) {
		for (int i = 0; i < textures.size(); i++) {
			std::string fname = textures[i]->GetName();
			if (fname.compare(filename) == 0 && textures[i]->GetMipmap() == Mipmaps) return i;
		}
		return -1;
	}

	unsigned int TextureManager::RequestTexture(const char* filename, bool hasMipmaps) {

		//Check if already exists
		int index = findTexture(filename, hasMipmaps);
		if (index != -1) return textures[index]->GetHandle();
		
		Texture* texture=new Texture(filename, hasMipmaps);
		texture->LoadImage();
		textures.push_back(texture);
		return texture->GetHandle();
		
	}

}

