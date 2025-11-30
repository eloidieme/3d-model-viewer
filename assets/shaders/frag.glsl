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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float materialShininess;
uniform vec3 lightPos;
uniform vec4 plane;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 specularMapColor = vec3(texture(texture_specular1, TexCoord));

    vec3 specular = 0.5 * spec * specularMapColor;

    vec4 objectColor = texture(texture_diffuse1, TexCoord);
    vec3 result = (ambient + diffuse) * objectColor.rgb + specular;

    float dist = dot(FragPos, plane.xyz) + plane.w; 

    if (dist > 0) discard;

    FragColor = vec4(result, objectColor.a);
}