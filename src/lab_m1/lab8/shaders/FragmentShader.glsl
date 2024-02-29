#version 330

// Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

//uniform vec3 light_direction2;
uniform vec3 light_position2;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int lightType;
uniform float angle;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 L2 = normalize(light_position2 - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 H2 = normalize(L2 + V);
	vec3 R = normalize(reflect(L, world_normal));

	// Define ambient light component
	float ambient_light = 0.35f;

	int color = 5;

	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(normalize(N), L), 0.f);
	float diffuse_light2 = material_kd * max(dot(normalize(N), L2), 0.f);

	// Compute specular light component
	float specular_light = 0.f;
	float specular_light2 = 0.f;

	if (diffuse_light > 0.f)
	{
		specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
	}

	if (diffuse_light2 > 0.f)
	{
		specular_light2 = material_ks * pow(max(dot(N, H2), 0), material_shininess);
	}

	// Compute light
	float light = 0.f;
	float light2 = ambient_light + diffuse_light2 + specular_light2;

	if (lightType == 1)
	{
		float cut_off_rad = radians(angle);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off_rad);
		
		if (spot_light > spot_light_limit)
		{	 
			float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			float light_att_factor = linear_att * linear_att;
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else
		{
			light = ambient_light;
		}
	} else {
		float d	= distance(light_position, world_position);
		float attenuation_factor = 1.f / max(d * d, 1.f);
		light = ambient_light + attenuation_factor * (diffuse_light + specular_light);
	}

	float toon = max(ceil(2 * diffuse_light * float(color)) / float(color), 0);

	// Write pixel out color
	vec3 colour = 3 * toon * object_color * light;

	out_color = vec4(colour, 1.f);
}