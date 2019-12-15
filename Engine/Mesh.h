#pragma once
#ifndef MESH_H
#define MESH_H

#include "glm.hpp"
#include <string>
#include <vector>
#include "Core.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "TextureManager.h"


namespace Engine {

	/**
	* Struct for representing a Material
	*	code for this struct is same with the Lab (Computer Graphics at AUEB 2019-2020)
	*/
	struct ENGINE_API Material {
		/**
		* Material's name
		*/
		std::string name;
		/**
		* Ambient values of the Material
		*/
		float ambient[4];
		/**
		* Diffuse values of the Material
		*/
		float diffuse[4];
		/**
		* Specular values of the Material
		*/
		float specular[4];
		/**
		* Alpha value
		*/
		float alpha;
		/**
		* shininess
		*/
		float shininess;

		int illumination_model;

		//textures
		std::string texture;
		std::string textureSpecular;
		std::string textureAmbient;
		std::string textureBump;
		std::string textureSpecularity;
		std::string textureOpacity;

		/**
		* Default Contructor for initializing a default Material
		*/
		Material() {
			ambient[0] = 0.2f;
			ambient[1] = 0.2f;
			ambient[2] = 0.2f;
			ambient[3] = 1.0f;

			diffuse[0] = 0.8f;
			diffuse[1] = 0.8f;
			diffuse[2] = 0.8f;
			diffuse[3] = 1.0f;

			specular[0] = 0.0f;
			specular[1] = 0.0f;
			specular[2] = 0.0f;
			specular[3] = 1.0f;

			alpha = 1;
			shininess = 0;
			illumination_model = 0;
		}
	};
	
	/**
	* Struct for representing a Face (aka 3 Points)
	*	code for this struct is same with the Lab (Computer Graphics at AUEB 2019-2020)
	*/
	struct ENGINE_API Face {
		/**
		* The three positions for it's point of our Face
		*/
		glm::ivec3 vertices;
		/**
		* The three normals for it's point of our Face
		*/
		glm::ivec3 normals;
		/**
		* The three Texture Coordinates for it's point of our Face
		*/
		glm::ivec3 textcoords;
	};

	/**
	* Struct for representing a Part
	*	code for this struct is same with the Lab (Computer Graphics at AUEB 2019-2020)
	*/
	struct ENGINE_API Part {
		unsigned int start_offset;
		unsigned int count;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		unsigned int textureID;
	};

	/**
	* Struct for representing a Object
	*	code for this struct is same with the Lab (Computer Graphics at AUEB 2019-2020)
	*/
	struct ENGINE_API Object {
		int material_id;
		unsigned int start;
		unsigned int end;
		std::string name;
	};

	/**
	* This class is representing Geometric Mesh
	*	The code for the implementation of this class is inspired by the Labs (Computer Graphics at AUEB 2019-2020)
	*/
	class ENGINE_API Mesh {
	protected:
		/**
		* Placeholder for the positions of the Mesh
		*/
		float* vertices;
		/**
		* Placeholder for the normals of the Mesh
		*/
		float* normals;
		/**
		* Placeholder fot the texture Coordinates of the Mesh
		*/
		float *textCoords;

		size_t pos_index, norm_index, text_index;


		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;


		VertexBuffer testPos, testNorm, testTexts, testTanagents;
		VertexArray testVa;

	public:

		/**
		* Default Contructor useful for initializations of the fields
		*/
		Mesh(void);
		
		/**
		* Destructor Empties memory both in heap and in GPU 
		*/
		~Mesh(void);

		/**
		* Allocate memory for data on the heap
		*/
		void setmem(size_t faces_size, bool hasTextures);

		/**
		* Free memory allocated on the heap
		*/
		void freemem(void);

		/**
		* Useful for filling the the positions Placeholder with data
		* @param Value to be assign to the next free spot of the Placeholder
		*/
		void setNextPosition(float value);
		/**
		* Useful for filling the the normals Placeholder with data
		* @param Value to be assign to the next free spot of the Placeholder
		*/
		void setNextNormal(float value);
		/**
		* Useful for filling the the texture Coordinates Placeholder with data
		* @param Value to be assign to the next free spot of the Placeholder
		*/
		void setNextTexture(float value);

		void setupBuffers(void);

		/**
		* Bind the Mesh (aka bind the associated VertexArray)
		*/
		void Bind(void) const;
		/**
		* Unbind the Mesh (aka unbind the associated VertexArray)
		*/
		void Unbind(void) const;

		/**
		* Find a material using it's name
		* @param str name of the material you are searching
		* @return A pointer to a material if it is found, NULL otherwise
		*/
		Material* findMaterial(std::string str);
		/**
		* Find the ID of material using it's name
		* @param str name of the material you are searching
		* @return The ID of the material if it was found, -1 otherwise
		*/
		int findMaterialID(std::string str);

		std::vector<Object> objects;
		std::vector<Material> materials;

		/**
		* Placeholder for more complex objects
		*/
		std::vector<Part> parts;

		size_t Vertices(void) { return pos_index / 3; }
		size_t Normals(void) { return norm_index / 3; }

		void calculate_tangents(void);

		//static TextureManager* textmanager;

	};

	
}

#endif//!MESH_H