#version 330 core
out vec4 fragColor;

in vec3 normalData;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    float ambientStrength = 0.1f;
    vec3 ambient = lightColor * objectColor;

    vec3 norm = normalize(normalData);
    vec3 lightDirection = normalize(lightPos - fragPos);
    float thetaDifference = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = thetaDifference * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;


    fragColor = vec4(result, 0.0f);
}
