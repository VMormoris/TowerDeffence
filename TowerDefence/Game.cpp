#include "Game.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"
#include <iostream>
#include "Loader.h"

Game::Game(void) : Renderer() {

	//m_sphere_vao = m_sphere_vvbo = m_sphere_nvbo = 0;

}

Game::~Game() {
	// delete g_buffer
	//glDeleteBuffers(1, &m_sphere_vao);
	//glDeleteBuffers(1, &m_sphere_vvbo);
	//glDeleteBuffers(1, &m_sphere_nvbo);
	delete knossos;
	delete skeleton;
	delete pirate;
}

void Game::Update(float dt) {
	Renderer::Update(dt);
	//std::cout << m_countinous_time << std::endl;
	skeleton->Update(m_countinous_time);
}

bool Game::InitCommonItems(void) {
	
	return true;
}

bool Game::InitRenderingTechniques(void) {

	bool initialized = true;

	std::string vertex_shader_path = "../Data/Shaders/basic_rendering.vert";
	std::string fragment_shader_path = "../Assets/Shaders/spotlight_rendering.frag";
	m_program.setVertexShader(vertex_shader_path.c_str());
	m_program.setFragmentShader(fragment_shader_path.c_str());
	m_program.CreateProgram();
	m_program.LoadUniform("uniform_projection_matrix");
	m_program.LoadUniform("uniform_view_matrix");
	m_program.LoadUniform("uniform_model_matrix");
	m_program.LoadUniform("uniform_normal_matrix");
	m_program.LoadUniform("uniform_diffuse");
	m_program.LoadUniform("uniform_specular");
	m_program.LoadUniform("uniform_shininess");
	m_program.LoadUniform("uniform_has_texture");
	m_program.LoadUniform("diffuse_texture");
	m_program.LoadUniform("uniform_camera_position");
	// Light Source Uniforms
	m_program.LoadUniform("uniform_light_projection_matrix");
	m_program.LoadUniform("uniform_light_view_matrix");
	m_program.LoadUniform("uniform_light_position");
	m_program.LoadUniform("uniform_light_direction");
	m_program.LoadUniform("uniform_light_color");
	m_program.LoadUniform("uniform_light_umbra");
	m_program.LoadUniform("uniform_light_penumbra");
	m_program.LoadUniform("uniform_light_exponent");
	m_program.LoadUniform("uniform_cast_shadows");
	m_program.LoadUniform("shadowmap_texture");

	return initialized;

}

bool Game::InitIntermediateShaderBuffers(void)
{
	return ResizeBuffers(m_width, m_height);
}

bool Game::InitGeometricMeshes()
{
	bool initialized = true;

	//sphere = Engine::loadObj("../Data/Knossos/knossos.obj");
	//sphere->setupBuffers();
	knossos = new Knossos();
	skeleton = new Skeleton();
	pirate = new Pirate();

	return initialized;
}

void Game::RenderGeometry(void) {

	/* Render here */
	// render to screen **********************************************
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(0, 0, m_width, m_height));

	// clear color and depth buffer
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_DEPTH_TEST));

	m_program.Bind();
	GLCall(glUniformMatrix4fv(m_program["uniform_projection_matrix"], 1, GL_FALSE, glm::value_ptr(cam.GetProjectionMatrix())));
	GLCall(glUniformMatrix4fv(m_program["uniform_view_matrix"], 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix())));
	glm::vec3 camera_position=cam.GetCameraCoords();
	GLCall(glUniform3f(m_program["uniform_camera_position"], camera_position.x, camera_position.y, camera_position.z));

	// pass the light source parameters
//#define DIRECTIONAL_LIGHT_SOURCE
#ifdef DIRECTIONAL_LIGHT_SOURCE
	glm::vec3 light_direction = glm::normalize(glm::vec3(0, 2, 0));
	glm::vec3 light_color = 1.2f * glm::vec3(1);// glm::vec3(1, 0.4, 0.4);
	glUniform3f(m_program["uniform_light_direction"], light_direction.x, light_direction.y, light_direction.z);
	glUniform3f(m_program["uniform_light_color"], light_color.x, light_color.y, light_color.z);
#endif

#define SPOT_LIGHT_SOURCE
#ifdef SPOT_LIGHT_SOURCE
	glm::vec3 spot_light_position(12, 18, -3);
	glm::vec3 target(0, 2, 0);
	glm::vec3 spot_light_direction = glm::normalize(target - spot_light_position);
	glm::vec3 spot_light_color = 40.0f * glm::vec3(255, 255, 251) / 255.f;
	float umbra = 50;
	float penumbra = 70;
	glUniform3f(m_program["uniform_light_position"], spot_light_position.x, spot_light_position.y, spot_light_position.z);
	glUniform3f(m_program["uniform_light_direction"], spot_light_direction.x, spot_light_direction.y, spot_light_direction.z);
	glUniform3f(m_program["uniform_light_color"], spot_light_color.x, spot_light_color.y, spot_light_color.z);
	glUniform1f(m_program["uniform_light_umbra"], umbra);
	glUniform1f(m_program["uniform_light_penumbra"], penumbra);
#endif

	// Enable Texture Unit 0
	GLCall(glUniform1i(m_program["uniform_diffuse_texture"], 0));
	GLCall(glActiveTexture(GL_TEXTURE0));

	
	knossos->Draw(m_program);
	skeleton->Draw(m_program);
	pirate->Draw(m_program);

	m_program.Unbind();

	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}