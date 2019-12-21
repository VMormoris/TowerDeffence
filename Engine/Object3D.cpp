#include "engpch.h"
#include "Object3D.h"

namespace Engine {

	Object3D::Object3D(const char* filename) {
		extra_transformation = false;
		Loader loader;
		mesh = loader.load(filename);
		mesh->Init();
		unique_rotation = 0.f;
	}

	Object3D::~Object3D() { delete mesh; }

	void Object3D::setExtra(float angle, glm::vec3 pivot, glm::vec3 rotvec) {
		unique_rotation = angle;
		center_pivot = pivot;
		rotation_vector = rotvec;
		extra_transformation = true;
	}

	void Object3D::Draw(Shader& program, float angle, glm::vec3 center, glm::vec3 rotvec, glm::vec3 scale) {
		//std::cout << "Drawing ... " << std::endl;
		glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.f), center) * glm::rotate(glm::mat4(1.f), angle, rotvec);
		if (extra_transformation) transformation_matrix = transformation_matrix * glm::translate(glm::mat4(1.f), center_pivot) * glm::rotate(glm::mat4(1.f), unique_rotation, rotation_vector);
		transformation_matrix = transformation_matrix * glm::scale(glm::mat4(1.f), scale);
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(transformation_matrix));

		mesh->Bind();//Bind Meshe's VertexArray
		
		
		GLCall(glUniformMatrix4fv(program["uniform_model_matrix"], 1, GL_FALSE, glm::value_ptr(transformation_matrix)));
		GLCall(glUniformMatrix4fv(program["uniform_normal_matrix"], 1, GL_FALSE, glm::value_ptr(normal_matrix)));
		for (int j = 0; j < mesh->parts.size(); j++)
		{
			glm::vec3 diffuseColor = mesh->parts[j].diffuseColor;
			glm::vec3 specularColor = mesh->parts[j].specularColor;
			float shininess = mesh->parts[j].shininess;

			GLCall(glUniform3f(program["uniform_diffuse"], diffuseColor.r, diffuseColor.g, diffuseColor.b));
			GLCall(glUniform3f(program["uniform_specular"], specularColor.r, specularColor.g, specularColor.b));
			GLCall(glUniform1f(program["uniform_shininess"], shininess));
			GLCall(glUniform1f(program["uniform_has_texture"], (mesh->parts[j].textureID > 0) ? 1.0f : 0.0f));
			GLCall(glBindTexture(GL_TEXTURE_2D, mesh->parts[j].textureID));

			GLCall(glDrawArrays(GL_TRIANGLES, mesh->parts[j].start_offset, mesh->parts[j].count));
		}
	}


	void Object3D::DrawShadowmap(Shader& program, float angle, glm::vec3 center, glm::vec3 rotvec, glm::vec3 scale) {

		glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.f), center) * glm::rotate(glm::mat4(1.f), angle, rotvec);
		if (extra_transformation) transformation_matrix = transformation_matrix * glm::translate(glm::mat4(1.f), center_pivot) * glm::rotate(glm::mat4(1.f), unique_rotation, rotation_vector);
		transformation_matrix = transformation_matrix * glm::scale(glm::mat4(1.f), scale);

		mesh->Bind();

		GLCall(glUniformMatrix4fv(program["uniform_model_matrix"], 1, GL_FALSE, glm::value_ptr(transformation_matrix)));
		for (int j = 0; j < mesh->parts.size(); j++) {
			GLCall(glDrawArrays(GL_TRIANGLES, mesh->parts[j].start_offset, mesh->parts[j].count));
		}

	}

	glm::vec3 Object3D::getMinOBB(void) { return mesh->minOBB; }

	glm::vec3 Object3D::getMaxOBB(void) { return mesh->maxOBB; }




}