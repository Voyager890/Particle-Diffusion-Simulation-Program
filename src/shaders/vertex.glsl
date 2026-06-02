#version 330 core
layout (location = 0) in vec3 positionData;
layout (location = 1) in vec3 verticesNormalData;

out vec3 normalData;
out vec3 fragPos;

uniform mat4 motion;
uniform mat4 camera;
uniform mat4 projection;

void main(){
    gl_Position = projection * camera * motion * vec4(positionData, 1.0f);
  
    fragPos = vec3(motion * vec4(positionData, 1.0f));
    normalData = mat3(transpose(inverse(motion))) * verticesNormalData;
}