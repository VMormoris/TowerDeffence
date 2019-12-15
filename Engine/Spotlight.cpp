#include "Spotlight.h"
#include "gtc\matrix_transform.hpp"
#include "utils.h"

namespace Engine {
	// Spot Light
	SpotLight::SpotLight()
	{
		m_name = "defaultSpotLight1";

		SetColor(glm::vec3(1.0f));
		SetPosition(glm::vec3(5, 3, 0));
		SetTarget(glm::vec3(0, 0, 0));
		// umbra and penumbra in degrees
		SetConeSize(60, 60);

		m_cast_shadow = false;
		m_shadow_map_resolution = 1024;
		m_shadow_map_bias = 0.001;
		//shadowmap_fbo = NULL;
	}

	SpotLight::~SpotLight(){}

	void SpotLight::CastShadow(bool cast)
	{
		m_cast_shadow = cast;

		if (cast)
		{
			/*
			if (m_shadow_map_texture == 0) {
				GLCall(glGenTextures(1, &m_shadow_map_texture));
			}
			// Depth buffer
			GLCall(glBindTexture(GL_TEXTURE_2D, m_shadow_map_texture));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_shadow_map_resolution, m_shadow_map_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			*/
			Texture* shadowmap_texture = new Texture("shadowmap_texture", false);
			shadowmap_texture->Generate();
			shadowmap_texture->Bind();
			shadowmap_texture->SetTextureImage(GL_DEPTH_COMPONENT24, m_shadow_map_resolution, m_shadow_map_resolution, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			shadowmap_texture->setParameters<int>(GL_CLAMP_TO_EDGE, GL_NEAREST);
			shadowmap_texture->Unbind();
			/*

			if (m_shadow_map_fbo == 0) {
				GLCall(glGenFramebuffers(1, &m_shadow_map_fbo));
			}
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo));
			GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadow_map_texture, 0));

			
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));*/
			shadowmap_fbo.Generate();
			shadowmap_fbo.AddDepthTexture(shadowmap_texture);
			shadowmap_fbo.Bind();
			GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowmap_texture->GetHandle(), 0));
			shadowmap_fbo.Unbind();
		}
	}

	void SpotLight::SetColor(glm::vec3 color)
	{
		m_light_color = color;
	}

	void SpotLight::SetPosition(glm::vec3 pos)
	{
		m_light_position = pos;
		m_light_direction = glm::normalize(m_light_target - m_light_position);
		m_view_matrix = glm::lookAt(m_light_position, m_light_target, glm::vec3(0, 1, 0));
		m_view_inverse_matrix = glm::inverse(m_view_matrix);
	}

	void SpotLight::SetTarget(glm::vec3 target)
	{
		m_light_target = target;
		m_light_direction = glm::normalize(m_light_target - m_light_position);
		m_view_matrix = glm::lookAt(m_light_position, m_light_target, glm::vec3(0, 1, 0));
		m_view_inverse_matrix = glm::inverse(m_view_matrix);
	}

	void SpotLight::SetConeSize(float umbra, float penumbra)
	{
		m_umbra = umbra;
		m_penumbra = penumbra;

		float near_clipping_range = 0.1f;
		float far_clipping_range = 1500.f;

		// create a frustum that  encapsulate the light cone
		float h = near_clipping_range * glm::tan(glm::radians(m_penumbra * 0.5f));
		m_projection_matrix = glm::frustum(-h, h, -h, h, near_clipping_range, far_clipping_range);
		m_projection_inverse_matrix = glm::inverse(m_projection_matrix);
	}

	glm::vec3 SpotLight::GetPosition()
	{
		return m_light_position;
	}

	glm::vec3 SpotLight::GetDirection()
	{
		return m_light_direction;
	}

	glm::vec3 SpotLight::GetColor()
	{
		return m_light_color;
	}

	float SpotLight::GetUmbra()
	{
		return m_umbra;
	}

	float SpotLight::GetPenumbra()
	{
		return m_penumbra;
	}

	bool SpotLight::GetCastShadowsStatus()
	{
		return m_cast_shadow;
	}

	void SpotLight::Bind(void)const { shadowmap_fbo.Bind(); }
	void SpotLight::Unbind(void)const { shadowmap_fbo.Unbind(); }
	void SpotLight::BindShadowmapTexture(void) const { shadowmap_fbo.BindDepthTexture(); }

	int SpotLight::GetShadowMapResolution()
	{
		return m_shadow_map_resolution;
	}

	glm::mat4 SpotLight::GetProjectionMatrix()
	{
		return m_projection_matrix;
	}

	glm::mat4 SpotLight::GetViewMatrix()
	{
		return m_view_matrix;
	}

}