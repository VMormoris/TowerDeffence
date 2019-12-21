#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H


#include "utils.h"
#include "FrameBuffer.h"

namespace Engine {
	class ENGINE_API SpotLight {
		std::string m_name;
		glm::vec3 m_light_direction;
		glm::vec3 m_light_position;
		glm::vec3 m_light_target;
		glm::vec3 m_light_color;

		float m_umbra;
		float m_penumbra;

		bool m_cast_shadow;
		int m_shadow_map_resolution;
		float m_shadow_map_bias;

		FrameBuffer shadowmap_fbo;

		glm::mat4 m_projection_matrix;
		glm::mat4 m_projection_inverse_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_inverse_matrix;

	public:
		SpotLight();
		~SpotLight();

		void SetPosition(glm::vec3 pos);
		void SetColor(glm::vec3 color);
		void SetTarget(glm::vec3 target);
		void SetConeSize(float umbra, float penumbra);
		void CastShadow(bool enable);

		glm::vec3 GetPosition();
		glm::vec3 GetDirection();
		glm::vec3 GetColor();

		float GetUmbra();
		float GetPenumbra();
		bool GetCastShadowsStatus();
		void Bind()const;
		void Unbind()const;
		void BindShadowmapTexture(void)const;
		int GetShadowMapResolution();

		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
	};
}

#endif // !SPOT_LIGHT_H
