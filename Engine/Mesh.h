#ifndef GEOMETRIC_MESH_H
#define GEOMETRIC_MESH_H

#include "engpch.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Loader.h"

namespace Engine {

	struct Part
	{
		unsigned int start_offset;
		unsigned int count;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		GLuint textureID;
		GLuint normalMapID;
	};

	class Mesh
	{
	public:
		Mesh(void);
		~Mesh(void);

		VertexBuffer* vvbo;
		VertexBuffer* nvbo;
		VertexBuffer* tvbo;
		VertexBuffer* tanvbo;
		VertexArray* vao;

		glm::vec3 minOBB;
		glm::vec3 maxOBB;

		void Init(void);

		struct Material* findMaterial(std::string str);
		int findMaterialID(std::string str);

		void Bind(void) const;
		void Unbind(void) const;

		/// test functions
		void printObjects(void);
		void printMaterials(void);

		// variables
		struct Object
		{
			int material_id;
			unsigned int start;
			unsigned int end;
			std::string name;
		};
		std::vector<Object> objects;
		std::vector<Material> materials;
		std::vector<Part> parts;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureCoord;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
	};
}
#endif