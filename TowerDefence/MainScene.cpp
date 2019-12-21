#include "MainScene.h"

MainScene::MainScene(Engine::Camera* camera) : Scene(camera) {}

MainScene::~MainScene() {}

void MainScene::Update(float dt) {
	Scene::Update(dt);
	if (!paused) { for (Engine::GameObject* object : gmobjects) object->Update(timestamp); }
}


bool MainScene::InitRenderingTechniques(void) {
	bool initialized = true;
	// Geometry Rendering Program
	std::string vertex_shader_path = "../Data/Shaders/basic_rendering.vert";
	std::string fragment_shader_path = "../Data/Shaders/basic_rendering.frag";
	geometry_program.setVertexShader(vertex_shader_path.c_str());
	geometry_program.setFragmentShader(fragment_shader_path.c_str());
	initialized = geometry_program.CreateProgram();
	geometry_program.LoadUniform("uniform_projection_matrix");
	geometry_program.LoadUniform("uniform_view_matrix");
	geometry_program.LoadUniform("uniform_model_matrix");
	geometry_program.LoadUniform("uniform_normal_matrix");
	geometry_program.LoadUniform("uniform_diffuse");
	geometry_program.LoadUniform("uniform_specular");
	geometry_program.LoadUniform("uniform_shininess");
	geometry_program.LoadUniform("uniform_has_texture");
	geometry_program.LoadUniform("diffuse_texture");
	geometry_program.LoadUniform("uniform_camera_position");
	// Light Source Uniforms
	geometry_program.LoadUniform("uniform_light_projection_matrix");
	geometry_program.LoadUniform("uniform_light_view_matrix");
	geometry_program.LoadUniform("uniform_light_position");
	geometry_program.LoadUniform("uniform_light_direction");
	geometry_program.LoadUniform("uniform_light_color");
	geometry_program.LoadUniform("uniform_light_umbra");
	geometry_program.LoadUniform("uniform_light_penumbra");
	geometry_program.LoadUniform("uniform_cast_shadows");
	geometry_program.LoadUniform("shadowmap_texture");

	// Post Processing Program
	vertex_shader_path = "../Data/Shaders/postproc.vert";
	fragment_shader_path = "../Data/Shaders/postproc.frag";
	postprocces_program.setVertexShader(vertex_shader_path.c_str());
	postprocces_program.setFragmentShader(fragment_shader_path.c_str());
	initialized = initialized && postprocces_program.CreateProgram();
	postprocces_program.LoadUniform("uniform_texture");
	postprocces_program.LoadUniform("uniform_time");
	postprocces_program.LoadUniform("uniform_depth");
	postprocces_program.LoadUniform("uniform_projection_inverse_matrix");

	// Shadow mapping Program
	vertex_shader_path = "../Data/Shaders/shadow_map_rendering.vert";
	fragment_shader_path = "../Data/Shaders/shadow_map_rendering.frag";
	shadowmap_program.setVertexShader(vertex_shader_path.c_str());
	shadowmap_program.setFragmentShader(fragment_shader_path.c_str());
	initialized = initialized && shadowmap_program.CreateProgram();
	shadowmap_program.LoadUniform("uniform_projection_matrix");
	shadowmap_program.LoadUniform("uniform_view_matrix");
	shadowmap_program.LoadUniform("uniform_model_matrix");

	return initialized;
}

bool MainScene::InitGeometricMeshes(void) {
	bool initialized = true;
	Engine::GameObject* knossos = new Field();
	gmobjects.push_back(knossos);
	//dispatchers.push_back(knossos);
	//objects.push_back(new Skeleton());
	Pirate* pirate = new Pirate();
	gmobjects.push_back(pirate);
	listeners.push_back(pirate);
	return initialized;
}

bool MainScene::InitLightSources(void) {
	lamp.SetPosition(glm::vec3(8, 14, -3));
	lamp.SetTarget(glm::vec3(0, 4, 0));
	lamp.SetColor(40.f * glm::vec3(255, 255, 251) / 255.f);
	lamp.CastShadow(true);
	return true;
}

void MainScene::RenderShadowmap(void) {
	if (lamp.GetCastShadowsStatus()) {
		int resolution = lamp.GetShadowMapResolution();

		lamp.Bind();//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, lamp.GetShadowMapFBO()));
		GLCall(glViewport(0, 0, resolution, resolution));
		unsigned int drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
		GLCall(glDrawBuffers(1, drawbuffers));

		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));

		shadowmap_program.Bind();
		glUniformMatrix4fv(shadowmap_program["uniform_projection_matrix"], 1, GL_FALSE, glm::value_ptr(lamp.GetProjectionMatrix()));
		glUniformMatrix4fv(shadowmap_program["uniform_view_matrix"], 1, GL_FALSE, glm::value_ptr(lamp.GetViewMatrix()));

		for (Engine::GameObject* object : gmobjects) { object->DrawShadowmap(shadowmap_program); }

		shadowmap_program.Unbind();

		GLCall(glDisable(GL_DEPTH_TEST));
		lamp.Unbind();//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}

void MainScene::RenderOutFB(void) {

	// Bind the default framebuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(0, 0, width, height));

	// clear the color and depth buffer
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// disable depth testing and blending
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDisable(GL_BLEND));

	// bind the post processing program
	postprocces_program.Bind();

	fvao.Bind();

	// Bind the intermediate color image to texture unit 0
	GLCall(glActiveTexture(GL_TEXTURE0));
	intermidiate_buffer.BindColorTexture();
	GLCall(glUniform1i(postprocces_program["uniform_texture"], 0));

	// Bind the intermediate depth buffer to texture unit 1
	GLCall(glActiveTexture(GL_TEXTURE1));
	intermidiate_buffer.BindDepthTexture();
	GLCall(glUniform1i(postprocces_program["uniform_depth"], 1));

	GLCall(glUniform1f(postprocces_program["uniform_time"], timestamp));
	glm::mat4 projection_inverse_matrix = glm::inverse(cam->getProjectionMatrix());
	GLCall(glUniformMatrix4fv(postprocces_program["uniform_projection_inverse_matrix"], 1, GL_FALSE, glm::value_ptr(projection_inverse_matrix)));

	GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

	fvao.Unbind();

	// Unbind the post processing program
	postprocces_program.Unbind();

}

void MainScene::RenderGeometry(void) {

	intermidiate_buffer.Bind();
	GLCall(glViewport(0, 0, width, height));
	unsigned int drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, drawbuffers));

	// clear color and depth buffer
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_DEPTH_TEST));

	geometry_program.Bind();

	// pass camera parameters to uniforms
	glUniformMatrix4fv(geometry_program["uniform_projection_matrix"], 1, GL_FALSE, glm::value_ptr(cam->getProjectionMatrix()));
	glUniformMatrix4fv(geometry_program["uniform_view_matrix"], 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));
	glm::vec3 m_camera_position = cam->getCoordinates();
	glUniform3f(geometry_program["uniform_camera_position"], m_camera_position.x, m_camera_position.y, m_camera_position.z);

	// pass the light source parameters to uniforms
	glm::vec3 light_position = lamp.GetPosition();
	glm::vec3 light_direction = lamp.GetDirection();
	glm::vec3 light_color = lamp.GetColor();
	glUniformMatrix4fv(geometry_program["uniform_light_projection_matrix"], 1, GL_FALSE, glm::value_ptr(lamp.GetProjectionMatrix()));
	glUniformMatrix4fv(geometry_program["uniform_light_view_matrix"], 1, GL_FALSE, glm::value_ptr(lamp.GetViewMatrix()));
	glUniform3f(geometry_program["uniform_light_position"], light_position.x, light_position.y, light_position.z);
	glUniform3f(geometry_program["uniform_light_direction"], light_direction.x, light_direction.y, light_direction.z);
	glUniform3f(geometry_program["uniform_light_color"], light_color.x, light_color.y, light_color.z);
	glUniform1f(geometry_program["uniform_light_umbra"], lamp.GetUmbra());
	glUniform1f(geometry_program["uniform_light_penumbra"], lamp.GetPenumbra());
	glUniform1i(geometry_program["uniform_cast_shadows"], (lamp.GetCastShadowsStatus()) ? 1 : 0);
	// Set the sampler2D uniform to use texture unit 1
	glUniform1i(geometry_program["shadowmap_texture"], 1);
	// Bind the shadow map texture to texture unit 1
	glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, (lamp.GetCastShadowsStatus()) ? lamp.GetShadowMapDepthTexture() : 0);
	if (lamp.GetCastShadowsStatus()) lamp.BindShadowmapTexture();
	else { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }

	// Enable Texture Unit 0
	glUniform1i(geometry_program["uniform_diffuse_texture"], 0);
	glActiveTexture(GL_TEXTURE0);

	for (Engine::GameObject* object : gmobjects) { object->Draw(geometry_program); }

	geometry_program.Unbind();

	glDisable(GL_DEPTH_TEST);
	intermidiate_buffer.Unbind();

}