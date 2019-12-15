#include "Mesh.h"
#include "utils.h"
#include <iostream>
#include <fstream>




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

		testTexts.Generate();
		testTexts.FillBuffer(textCoords, text_index * sizeof(float));
		testVa.AddAttribute(twofloats);

		if (!tangents.empty()) { 
			testTanagents.Generate();
			testTanagents.FillBuffer(&tangents[0], tangents.size() * sizeof(glm::vec3));
			testVa.AddAttribute(threefloats);
			testVa.Unbind();
			testTanagents.Unbind();
		}
		else {
			testVa.Unbind();
			testTexts.Unbind();
		}

		for (int i = 0; i < objects.size(); i++) {
			Part part;
			part.start_offset = objects[i].start;
			part.count = objects[i].end - objects[i].start;

			Material material = materials[objects[i].material_id];

			part.diffuseColor = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.f);
			part.specularColor = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
			part.shininess = material.shininess;
			part.textureID = (material.texture.empty()) ? 0 : TextureManager::RequestTexture(material.texture.c_str());
			//printf("%d\n",part.textureID);
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

	void Mesh::calculate_tangents()
	{
		tangents.clear();

		for (unsigned int i = 0; i < Vertices(); i += 3)
		{
			glm::vec3 v0(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);//glm::vec3& v0 = mesh->vertices[i + 0];
			glm::vec3 v1(vertices[i * 3 + 3], vertices[i * 3 + 4], vertices[i * 3 + 5]);
			glm::vec3 v2(vertices[i * 3 + 6], vertices[i * 3 + 7], vertices[i * 3 + 8]);

			glm::vec2 uv0(textCoords[i + 0], textCoords[i + 1]);
			glm::vec2 uv1(textCoords[i + 2], textCoords[i + 3]);
			glm::vec2 uv2(textCoords[i + 4], textCoords[i + 5]);

			// edges of the triangle : position delta
			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;

			// uv delta
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			glm::vec3 b = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			// set the same tangent for all the three vertices of the triangle.
			// they will be merged later

			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);

			bitangents.push_back(b);
			bitangents.push_back(b);
			bitangents.push_back(b);
		}

		for (unsigned int i = 0; i < Vertices(); i += 3)
		{
			glm::vec3 n (normals[i+0], normals[i+1], normals[i+2]);
			glm::vec3& t = tangents[i/3];
			glm::vec3& b = bitangents[i/3];

			// Gram-Schmidt orthogonalize
			t = glm::normalize(t - n * glm::dot(n, t));

			// Calculate handedness
			if (glm::dot(glm::cross(n, t), b) < 0.0f) {
				t = t * -1.0f;
			}

		}
	}


}
