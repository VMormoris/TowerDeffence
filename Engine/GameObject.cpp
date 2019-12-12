#include "GameObject.h"
#include "gtc/type_ptr.hpp"

namespace Engine {

	void GameObject::Draw(Shader& program) {
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

}