#version 330 core
layout(location = 0) out vec4 out_color;

uniform vec3 uniform_diffuse;
uniform vec3 uniform_specular;
uniform float uniform_shininess;
uniform float uniform_has_texture;
uniform sampler2D diffuse_texture;

// Camera Properties
uniform vec3 uniform_camera_position;

// Light Source Properties
uniform vec3 uniform_light_direction;
uniform vec3 uniform_light_color;

in vec2 f_texcoord;
in vec3 f_position_wcs;
in vec3 f_normal;

#define PI 3.14159


void main(void){
  vec3 normal = normalize(f_normal);
  
	vec4 diffuseColor = vec4(uniform_diffuse.rgb, 1);
	// if we provide a texture, multiply color with the color of the texture
	diffuseColor = mix( diffuseColor, diffuseColor * texture(diffuse_texture, f_texcoord), uniform_has_texture);
	
  // direction to the light source
	vec3 vertex_to_light_direction = uniform_light_direction;
	float dist = 1.0;

  // direction to the camera
	vec3 viewDirection = normalize(uniform_camera_position - f_position_wcs.xyz);
	// half vector
	vec3 halfVector = normalize(viewDirection + vertex_to_light_direction);

	float NdotL = max(dot(vertex_to_light_direction, normal), 0.0);
	float NdotH = max(dot(halfVector, normal), 0.0);

	// compute the incident radiance
	vec3 irradiance = uniform_light_color * NdotL / dist;

  // compute the diffuse reflection
	vec3 diffuseReflection = irradiance * diffuseColor.rgb / PI;

	// compute the specular reflection
	float specularNormalization = (uniform_shininess + 8) / (8 * PI);
	vec3 specularReflection = (NdotL > 0.0)? irradiance * specularNormalization * uniform_specular * pow( NdotH, uniform_shininess + 0.001) : vec3(0);

	out_color = vec4( diffuseReflection + specularReflection, 1.0);

}
