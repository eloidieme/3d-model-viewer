#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform CameraData {
    mat4 view;
    mat4 projection;
    vec3 viewPos;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;

void main() {
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}