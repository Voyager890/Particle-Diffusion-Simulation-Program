#version 330 core
layout (location = 0) in vec3 positionData;

uniform mat4 motion;
uniform mat4 camera;
uniform mat4 projection;

void main(){
    gl_Position = projection * camera * motion * vec4(positionData, 1.0f);
}