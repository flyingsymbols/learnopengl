#version 330 core
layout (location = 0) in vec3 vx_pos;
layout (location = 1) in vec3 vx_norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(vx_pos, 1.0);
    FragPos = vec3(model * vec4(vx_pos, 1.0));

    // Inversing matrices is a costly operation even for shaders so 
    // wherever possible, try to avoid doing inverse operations in shaders 
    // since they have to be done on each vertex of your scene. For learning 
    // purposes this is fine, but for an efficient application you'll likely 
    // want to calculate the normal matrix on the CPU and send it to the 
    // shaders via a uniform before drawing (just like the model matrix).
    Normal = mat3(transpose(inverse(model))) * vx_norm;
}
