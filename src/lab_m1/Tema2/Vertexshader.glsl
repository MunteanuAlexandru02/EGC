#version 330


// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int life;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;


void main()
{
    // TODO(student): Compute world space vertex position and normal,
    // and send them to the fragment shader
    world_position = vec3(Model * vec4(v_position, 1));
    world_normal = normalize( mat3(Model) * normalize(v_normal));

    

    if (life > 0) {
        float scale_x = 1.0 + 0.4 * 1.0 / (7 - float(life));
        float scale_y = 1.0;
        float scale_z = 1.0 + 0.4 * 1.0 / (7 - float(life));

        vec3 scaled_position = vec3(scale_x * v_position.x, scale_y * v_position.y, scale_z * v_position.z);
        gl_Position = Projection * View * Model * vec4(scaled_position, 1.0);
    }
    else
        gl_Position = Projection * View * Model * vec4(v_position , 1.0);
}
