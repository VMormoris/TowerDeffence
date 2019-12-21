#include "Loader.h"
#include "engpch.h"
#include "Mesh.h"
#include "TextureManager.h"

namespace Engine{

	Mesh::Mesh()
	{
		vvbo = nvbo = tvbo = tanvbo = NULL;
		vao = NULL;
	}

	void Mesh::Init(void) {
		Attribute threefloats(3);
		Attribute twofloats(2);

		vao = new VertexArray();
		vao->Generate();
		vao->Bind();

		vvbo = new VertexBuffer();
		vvbo->Generate();
		vvbo->FillBuffer(&vertices[0], vertices.size() * sizeof(glm::vec3));
		vao->AddAttribute(threefloats);

		nvbo = new VertexBuffer();
		nvbo->Generate();
		nvbo->FillBuffer(&normals[0], normals.size() * sizeof(glm::vec3));
		vao->AddAttribute(threefloats);

		tvbo = new VertexBuffer();
		tvbo->Generate();
		tvbo->FillBuffer(&textureCoord[0], textureCoord.size() * sizeof(glm::vec2));
		vao->AddAttribute(twofloats);

		if (!tangents.empty()) {
			tanvbo = new VertexBuffer();
			tanvbo->Generate();
			tanvbo->FillBuffer(&tangents[0], tangents.size() * sizeof(glm::vec3));
			vao->AddAttribute(threefloats);
			vao->Unbind();
			tanvbo->Unbind();
		}
		else {
			vao->Unbind();
			tvbo->Unbind();
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


	Material* Mesh::findMaterial(std::string str)
	{
		if (str.empty()) str = "default";
		for (unsigned int i = 0; i < materials.size(); i++)
		{
			if (materials[i].name == str) return &materials[i];
		}
		return NULL;
	}

	int Mesh::findMaterialID(std::string str)
	{
		if (str.empty()) str = "default";
		for (unsigned int i = 0; i < materials.size(); i++)
		{
			if (materials[i].name == str)
				return i;
		}
		return -1;
	}

	void Mesh::printObjects(void)
	{
		printf("\n          OBJECTS   :\n");
		printf("Vertices =%zu \nNormals =%zu \nTextureCoords=%zu\n", vertices.size(), normals.size(), textureCoord.size());
		printf("We loaded %zu meshes\n", objects.size());
		glm::vec3 minValue(FLT_MAX);
		glm::vec3 maxValue(-FLT_MAX);
		for (glm::vec3 vertex : vertices)
		{
			minValue = glm::min(minValue, vertex);
			maxValue = glm::max(minValue, vertex);
		}
		printf("Min %f %f %f", minValue.x, minValue.y, minValue.z);
		printf("Max %f %f %f", maxValue.x, maxValue.y, maxValue.z);
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			printf("\nObject %d ", i);
			if (objects[i].name.size() > 0) printf("%s \n", objects[i].name.c_str());
			else printf("\n");
			printf("From %d to %d with %d faces\n", objects[i].start, objects[i].end, (objects[i].end - objects[i].start) / 3);
			if (objects[i].material_id > 0) printf("Uses %d : %s material\n", objects[i].material_id, materials[objects[i].material_id].name.c_str());
			else printf("Uses NO material\n");
		}

	}
	void Mesh::printMaterials(void)
	{
		if (materials.empty()) printf(" NO MATERIALS \n");
		else printf("\n USED MATERIALS :\n");
		for (unsigned int i = 0; i < materials.size(); i++)
		{
			printf("%i ", i);
			printf("Diffuse : %f %f %f\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		}
	}

	Mesh::~Mesh(void) {
		if (vvbo != NULL) delete vvbo;
		if (nvbo != NULL) delete nvbo;
		if (tvbo != NULL) delete tvbo;
		if (tanvbo != NULL) delete tanvbo;
		if (vao != NULL) delete vao;
	}

	void Mesh::Bind(void) const { vao->Bind(); }
	void Mesh::Unbind(void) const { vao->Bind(); }

}