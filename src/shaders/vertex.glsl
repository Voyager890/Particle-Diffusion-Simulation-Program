#version 330 core
layout (location = 0) in vec3 PositionData;
layout (location = 1) in vec3 ColourData;

out vec4 FragmentColor;

uniform mat4 VertexTransformer;
uniform mat4 CameraInitialize;
uniform mat4 Projection;

void main(){
    gl_Position = Projection * CameraInitialize * VertexTransformer * vec4(PositionData, 1.0f);

    FragmentColor = vec4(ColourData, 1.0f);
}