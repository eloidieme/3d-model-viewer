#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

layout (std140) uniform CameraData {
    mat4 view;
    mat4 projection;
    vec3 viewPos;
};

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform float materialShininess;
uniform vec3 lightPos;

#define MAX_CLIPPING_PLANES 8
uniform int u_ActiveClippingPlanes;
uniform vec4 u_ClippingPlanes[MAX_CLIPPING_PLANES];

void main() {
    for (int i = 0; i < u_ActiveClippingPlanes; i++) {
        if (dot(vec4(FragPos, 1.0), u_ClippingPlanes[i]) < 0.0) discard;
    }

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 specularMapColor = vec3(texture(texture_specular, TexCoord));

    vec3 specular = 0.5 * spec * specularMapColor;
    vec4 objectColor = texture(texture_diffuse, TexCoord);
    vec3 result = (ambient + diffuse) * objectColor.rgb + specular;

    FragColor = vec4(result, objectColor.a);
}