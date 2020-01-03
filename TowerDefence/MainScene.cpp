#include "MainScene.h"
#include "Field.h"
#include "Road.h"

MainScene::MainScene(Engine::Camera* camera) : Scene(camera) {}

MainScene::~MainScene() {}

void MainScene::Update(float dt) {
	Scene::Update(dt);
	if (!paused) { 
		for (Engine::GameObject* object : gmobjects) object->Update(dt); 
		for (Tower* tower : towers) {
			for (Pirate* pirate : pirates) {
				float distance = glm::distance(pirate->getCoords(), tower->getCoords());
				if (distance < 10.f) {
					glm::vec3 target = pirate->CenterAt(timestamp + 1.f);
					target.y = 1.f;
					CannonBall* ball = tower->Fire(target, timestamp);
					if (ball != NULL) { gmobjects.push_back(ball); colliders.push_back(ball); }
				}
			}
		}
	}
}

void MainScene::DeleteNotUsedObjects(void) {
	std::vector<int> indexes;
	for (int i = 0; i < pirates.size(); i++) {
		if (pirates[i]->isForDelete()) { 
			indexes.push_back(i); 
		}
	}
	for (int i = (indexes.size() - 1); i >= 0; i--) {
		int index = indexes[i];
		pirates.erase(pirates.begin() + index);
	}
	indexes.clear();
	for (int i = 0; i < gmobjects.size(); i++) {
		Engine::GameObject* object = gmobjects[i];
		if (object->isForDelete()) indexes.push_back(i);
	}
	for (int i = (indexes.size() - 1); i >= 0; i--) {
		int index = indexes[i];
		delete gmobjects[index];
		gmobjects.erase(gmobjects.begin() + index);
	}
	indexes.clear();
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
	Engine::GameObject* field = new Field();
	gmobjects.push_back(field);
	Pirate* pirate = new Pirate();
	gmobjects.push_back(pirate);
	//listeners.push_back(pirate);
	colliders.push_back(pirate);
	pirates.push_back(pirate);
	Road* road = new Road();
	gmobjects.push_back(road);
	Tower* tower = new Tower();
	gmobjects.push_back(tower);
	towers.push_back(tower);
	return initialized;
}

bool MainScene::InitLightSources(void) {
	lamp.SetPosition(glm::vec3(16.f, 30.f, 16.f));
	lamp.SetTarget(glm::vec3(16.4f, 0.f, 16.f));
	lamp.SetColor(glm::vec3(140.f));
	lamp.SetConeSize(73, 80);
	lamp.CastShadow(true);
	return true;
}
static bool flag = true;

void MainScene::DetectCollisions(void) {
	if (colliders.size() <= 1) return;
	std::vector<int> indexes;
	for (int i = 0; i < colliders.size() - 1; i++) {
		for (int j = i + 1; j < colliders.size(); j++) {
			CollisionListener* collider1 = colliders[i];
			CollisionListener* collider2 = colliders[j];
			if (collider1->GetTag().compare("cannonball") == 0 && collider2->GetTag().compare("cannonball") == 0) {
				float distance = glm::distance(((CannonBall*)collider1)->getCoords(), ((CannonBall*)collider2)->getCoords());
				if (distance <= 0.2) collider1->onCollision(collider2);
			}
			else if ((collider1->GetTag().compare("cannonball") == 0 && collider2->GetTag().compare("pirate") == 0) || (collider1->GetTag().compare("pirate") == 0 && collider2->GetTag().compare("cannonball") == 0)) {
				Pirate* pirate;
				CannonBall* ball;
				int cdel_index = -1;
				int pdel_index = -1;
				if (collider1->GetTag().compare("cannonball") == 0) {
					pirate = (Pirate*)collider2;
					ball = (CannonBall*)collider1;
					cdel_index = i;
					pdel_index = j;
				}
				else {
					pirate = (Pirate*)collider1;
					ball = (CannonBall*)collider2;
					cdel_index = j;
					pdel_index = i;
				}
				glm::vec3* boxes = pirate->getBoxes();
				glm::vec3 ballCenter = ball->getCoords();
				for (int k = 0; k < 8; k += 2) {
					glm::vec3 min = boxes[k] - glm::vec3(0.1);
					glm::vec3 max = boxes[k + 1] + glm::vec3(0.1);

					if (ballCenter.x >= min.x && ballCenter.x <= max.x && ballCenter.y >= min.y && ballCenter.y <= max.y && ballCenter.z >= min.z && ballCenter.z <= max.z) {
						pirate->onCollision((CollisionListener*)ball);
						ball->DeleteASAP();
						indexes.push_back(cdel_index);
						if (pirate->isForDelete()) { 
							indexes.push_back(pdel_index);
						}
						break;
					}
				}
				delete[] boxes;
			}
		}
	}

	std::sort(indexes.begin(), indexes.end(), [](int i, int j) {return i > j; });

	for(int index: indexes)
		colliders.erase(colliders.begin() + index);
}

void MainScene::RenderShadowmap(void) {
	if (lamp.GetCastShadowsStatus()) {
		int resolution = lamp.GetShadowMapResolution();

		lamp.Bind();
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
		lamp.Unbind();
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