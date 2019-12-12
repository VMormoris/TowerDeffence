#include "Mesh.h"
#include "utils.h"
#include <iostream>
#include <fstream>


static Engine::TextureManager textmanager;

namespace Engine {

	Mesh::Mesh(void) {
		vertices = NULL;  pos_index = 0;
		normals = NULL;  norm_index = 0;
		textCoords = NULL; text_index = 0;
	}

	Mesh::~Mesh(void) {
		freemem();
	}

	void Mesh::setupBuffers(void) {
		Attribute threefloats(3);
		Attribute twofloats(2);
	

		testVa.Generate();
		testVa.Bind();
		
		testPos.Generate();
		testPos.FillBuffer(vertices, pos_index * sizeof(float));
		testVa.AddAttribute(threefloats);
		
		testNorm.Generate();
		testNorm.FillBuffer(normals, norm_index * sizeof(float));
		testVa.AddAttribute(threefloats);

		if (text_index > 0) {
			testTexts.Generate();
			testTexts.FillBuffer(textCoords, text_index * sizeof(float));
			testVa.AddAttribute(twofloats);
			testVa.Unbind();
			testTexts.Unbind();
		}
		else {
			testVa.Unbind();
			testNorm.Unbind();
		}

		for (int i = 0; i < objects.size(); i++) {
			Part part;
			part.start_offset = objects[i].start;
			part.count = objects[i].end - objects[i].start;

			Material material = materials[objects[i].material_id];

			part.diffuseColor = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.f);
			part.specularColor = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
			part.shininess = material.shininess;
			part.textureID = (material.texture.empty()) ? 0 : textmanager.RequestTexture(material.texture.c_str());
			parts.push_back(part);
		}
		
	}

	void Mesh::Bind(void) const { testVa.Bind();}

	void Mesh::Unbind(void) const { testVa.Unbind(); }

	void Mesh::setmem(size_t faces_size, bool hasTextures) {
		vertices = new float[faces_size * 3 * 3];
		normals = new float[faces_size * 3 * 3];
		if (hasTextures)
			textCoords = new float[faces_size * 2 * 3];
	}

	void Mesh::freemem(void) {
		if (vertices != NULL) delete[] vertices;
		if (normals != NULL) delete[] normals;
		if (textCoords != NULL) delete[] textCoords;
	}

	Material* Mesh::findMaterial(std::string str) {
		if (str.empty()) str = "default";
		for (int i = 0; i < materials.size(); i++) {
			if (materials[i].name == str) return &materials[i];
		}
		return NULL;
	}

	int Mesh::findMaterialID(std::string str) {
		if (str.empty()) str = "default";
		for (int i = 0; i < materials.size(); i++) {
			if (materials[i].name == str) return i;
		}
		return -1;
	}

	void Mesh::setNextPosition(float value) {
		vertices[pos_index] = value;
		pos_index++;
	}

	void Mesh::setNextNormal(float value) {
		normals[norm_index] = value;
		norm_index++;
	}
	void Mesh::setNextTexture(float value) {
		textCoords[text_index] = value;
		text_index++;
	}

}
