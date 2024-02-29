#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 texture_coords;
layout(location = 3) in vec3 v_color;



// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_position;
out vec2 frag_texture;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color + vec3(tan(Time), atan(Time), 1f); // pt 6 : v_normal
    frag_normal = v_normal; //+ 
    frag_position = v_position + vec3(tan(Time), 1/tan(Time) + cos(Time), sin(Time));
    frag_texture = texture_coords + vec2(tan(Time), sin(Time));



    // TODO(student): Compute gl_Position
    //gl_Position = Projection * View * Model * vec4(frag_position, 1.0);
    
    gl_Position =  Projection * View * Model *
    mat4(tan(Time), 0,0,0,
        0, sin(Time), 0, 0,
        0, 0, atan(Time), 0,
        0,0,0,1) * vec4(frag_position, 1.0);
    
}
