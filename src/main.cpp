// External Libraris
#include "glad/glad.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>
#include <math.h>
#include <cmath>

#include <new>
#include <ostream>

// Custom Libraries
#include "shaders/class.hpp"
#include "class_bufferObjects/bufferObjects.h"
#include "class_particles/particles.h"
#include "func_proceduralSphere/proceduralSphere.h"

int wWidth = 1280;
int wHeight = 720;

void programInit(int &iVerticesPerRing, float &iRadius);

void inputCheck(GLFWwindow* window, glm::mat4& matrix);
void resize_callback(GLFWwindow* window,int width, int height);

// void vertexRingGenerator(class_bufferObjects &bufferObjects);
// void primaryVertexInit(class_bufferObjects &bufferObjects);
// void elementBufferGenerator(class_bufferObjects &bufferObjects);

void debug_vboDataDisplayer(class_bufferObjects &bufferObjects);
void debug_eboDataDisplayer(class_bufferObjects &bufferObjects);

int main(){
    // Initialize the required parameters
    int iVerticesPerRing = 32;
    float iRadius = 1.0f;
    glm::vec3 lightSourceOrigin(0.0f, 0.0f, 0.0f);   
    glm::vec3 lightColor(1.0f);

    // programInit(iVerticesPerRing, iRadius);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "Diffusion Simulation", NULL, NULL);
    if(window == NULL){
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Glad initialization failed" << std::endl;
        return -1;
    }
    glViewport(0, 0, wWidth, wHeight);

    glfwSetFramebufferSizeCallback(window, resize_callback);
    
    
    
    class_bufferObjects bufferObjects(iVerticesPerRing, iRadius);
    vertexRingGenerator(bufferObjects);    
    debug_vboDataDisplayer(bufferObjects);
    debug_eboDataDisplayer(bufferObjects);

    class_particleType particleType_Orange(bufferObjects, glm::vec3(0.8f, 0.5f, 0.0f), 10.0f);

    class_particle* particle_Orange[particleType_Orange.particleCount]; 
    for(int i = 0; i < particleType_Orange.particleCount; i++){
        particle_Orange[i] = new class_particle;
        particle_Orange[i]->particleTypeLink = &particleType_Orange;
    }

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)wWidth/(float)wHeight, 0.1f, 100.0f);
    glm::mat4 cameraInit = glm::mat4(1.0f);
    glm::vec3 cameraPosition(0.0f, 0.0f, -6.0f);
    cameraInit = glm::translate(cameraInit, cameraPosition);
    
    
    Shader shader_standarad(SHADER_PATH"/vertex.glsl", SHADER_PATH"/fragment.glsl");    
    shader_standarad.use();
    shader_standarad.setMat4("motion", particle_Orange[0]->position);
    shader_standarad.setMat4("camera", cameraInit);
    shader_standarad.setMat4("projection", projectionMatrix);

    shader_standarad.setVec3("objectColor", particleType_Orange.objectColor);
    shader_standarad.setVec3("lightColor", lightColor);
    shader_standarad.setVec3("lightPos", lightSourceOrigin);
    
    Shader shader_lightSource(SHADER_PATH"/lightSourceVertex.glsl",SHADER_PATH"/lightSourceFragment.glsl");
    shader_lightSource.use();
    glm::mat4 lightSourceMotion(1.0f);
    lightSourceMotion = glm::translate(lightSourceMotion, lightSourceOrigin);
    shader_lightSource.setMat4("motion", lightSourceMotion);
    shader_lightSource.setMat4("camera", cameraInit);
    shader_lightSource.setMat4("projection", projectionMatrix);

    float lightSourceVertices[] = {0.0f, 1.0f, 0.0f,    1.0f,-0.5f,0.0f,    -1.0f,-0.5f,0.0f};
    unsigned int lightSourceVBO, lightSourceVAO;

    glGenVertexArrays(1, &lightSourceVAO);
    glGenBuffers(1, &lightSourceVBO);

    glBindVertexArray(lightSourceVAO);
    glBindBuffer(1, lightSourceVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(lightSourceVertices) / sizeof(float), lightSourceVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufferObjects.capacity * sizeof(float), bufferObjects.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferObjects.eboCapacity * sizeof(unsigned int), bufferObjects.eboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferObjects.stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, bufferObjects.stride * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wire Frame
    glEnable(GL_DEPTH_TEST);
    
    while(!glfwWindowShouldClose(window)){
        int current = 0; 
        inputCheck(window, particle_Orange[current]->position);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader_standarad.use();
        glBindVertexArray(vao);
        
        for(int i = 0; i < particleType_Orange.particleCount; i++){
            shader_standarad.setMat4("motion", particle_Orange[i]->position);
            glDrawElements(GL_TRIANGLES, bufferObjects.eboCapacity, GL_UNSIGNED_INT, 0);
        }

        shader_lightSource.use();
        glBindVertexArray(lightSourceVAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

void resize_callback(GLFWwindow* window, int width, int height){
    wWidth = width;
    wHeight = height;
    glViewport(0, 0, wWidth, wHeight);
}

void inputCheck(GLFWwindow *window, glm::mat4 &matrix){
    const float rotationSensitivity = 1.0f;
    const float translationSensitivity = 0.1f;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    // Yaw / Vertical
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){matrix = glm::rotate(matrix, glm::radians( rotationSensitivity), glm::vec3(0.0f, 1.0f, 0.0f));}
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){matrix = glm::rotate(matrix, glm::radians(-rotationSensitivity), glm::vec3(0.0f, 1.0f, 0.0f));}
    // Pitch / Lateral
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){matrix = glm::rotate(matrix, glm::radians( rotationSensitivity), glm::vec3(1.0f, 0.0f, 0.0f));}
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){matrix = glm::rotate(matrix, glm::radians(-rotationSensitivity), glm::vec3(1.0f, 0.0f, 0.0f));}
    // Roll / Longitudanal
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){matrix = glm::rotate(matrix, glm::radians( rotationSensitivity), glm::vec3(0.0f, 0.0f, 1.0f));}
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){matrix = glm::rotate(matrix, glm::radians(-rotationSensitivity), glm::vec3(0.0f, 0.0f, 1.0f));}

    // Translation
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -translationSensitivity));};
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, translationSensitivity));};
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){matrix = glm::translate(matrix, glm::vec3(translationSensitivity, 0.0f, 0.0f));};
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){matrix = glm::translate(matrix, glm::vec3(-translationSensitivity, 0.0f, 0.0f));};
    if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){matrix = glm::translate(matrix, glm::vec3(0.0f, translationSensitivity, 0.0f));};
    if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){matrix = glm::translate(matrix, glm::vec3(0.0f, -translationSensitivity, 0.0f));};
}
/*
void vertexRingGenerator(class_bufferObjects &bufferObjects){
    for(int currentLayer = 0; currentLayer < bufferObjects.layerCount; currentLayer++){
        const float pitch = glm::radians(90.0 - ((currentLayer + 1) * bufferObjects.displacementAngle));
        const float layerRadius = (cos(pitch) * bufferObjects.radius); 

        for(int step = 0; step < bufferObjects.verticesPerRingCount; step++){
            const float yaw = glm::radians(bufferObjects.displacementAngle * step);
            const int currentVertex = (bufferObjects.stride * step) + (bufferObjects.verticesPerRingCount * bufferObjects.stride * (currentLayer));

            // Calculate and scale vertex position to fit the radius bufferObjects
            bufferObjects.vertices[currentVertex]     = cos(yaw)   * layerRadius;      // X position Attribute    
            bufferObjects.vertices[currentVertex + 1] = sin(yaw)   * layerRadius;      // Y position Attribute
            bufferObjects.vertices[currentVertex + 2] = sin(pitch) * bufferObjects.radius;    // Z position Attribute
            
            // Setting the normalized vertex normal data attribute
            const double vectorLength = std::sqrt(bufferObjects.vertices[currentVertex] * bufferObjects.vertices[currentVertex] 
                                      + bufferObjects.vertices[currentVertex + 1] * bufferObjects.vertices[currentVertex + 1]
                                      + bufferObjects.vertices[currentVertex + 2] * bufferObjects.vertices[currentVertex + 2]);
            bufferObjects.vertices[currentVertex + 3] = bufferObjects.vertices[currentVertex]     / vectorLength;
            bufferObjects.vertices[currentVertex + 4] = bufferObjects.vertices[currentVertex + 1] / vectorLength;
            bufferObjects.vertices[currentVertex + 5] = bufferObjects.vertices[currentVertex + 2] / vectorLength;
            
            // Correcting for floating point precision error in angle to radians conversion for basis vector positions
            for(int i = 0; i < 3; ++i){
                bufferObjects.vertices[currentVertex + i] = std::abs(bufferObjects.vertices[currentVertex + i]) < 0.0001 ? 0.0f : bufferObjects.vertices[currentVertex + i]; 
                bufferObjects.vertices[currentVertex + i] = std::abs(bufferObjects.vertices[currentVertex + i]) > 0.9999 * bufferObjects.radius ? std::copysign(1.0 * bufferObjects.radius, bufferObjects.vertices[currentVertex + i]) : bufferObjects.vertices[currentVertex + i];
            }
        }    
    }

    primaryVertexInit(bufferObjects);
    elementBufferGenerator(bufferObjects);
}

void primaryVertexInit(class_bufferObjects &bufferObjects){
    float *newBuffer = new float[bufferObjects.capacity + (2 * bufferObjects.stride)]{0.0f};

    const int offset = bufferObjects.stride * 1; // How many starting elements to offset by 
    for(int i = 0; i < bufferObjects.capacity; ++i){newBuffer[i + offset] = bufferObjects.vertices[i];};
    
    // Position data
    newBuffer[0] = 0.0f * bufferObjects.radius;  
    newBuffer[1] = 0.0f * bufferObjects.radius;  
    newBuffer[2] = 1.0f * bufferObjects.radius;
    // Normal data  
    newBuffer[3] = 0.0f;  
    newBuffer[4] = 0.0f;  
    newBuffer[5] = 1.0f;
    
    // Second prime vertex
    const int lastVertexElementStart = bufferObjects.capacity + bufferObjects.stride; 
    // Position Data
    newBuffer[lastVertexElementStart]     =  0.0f * bufferObjects.radius;
    newBuffer[lastVertexElementStart + 1] =  0.0f * bufferObjects.radius;
    newBuffer[lastVertexElementStart + 2] = -1.0f * bufferObjects.radius;
    // Normal data
    newBuffer[lastVertexElementStart + 3] = 0.0f;
    newBuffer[lastVertexElementStart + 4] = 0.0f;
    newBuffer[lastVertexElementStart + 5] = -1.0f;


    bufferObjects.totalVerticesCount += 2;
    bufferObjects.capacity = bufferObjects.totalVerticesCount * bufferObjects.stride;
    delete[] bufferObjects.vertices;
    bufferObjects.vertices = newBuffer;
}

void elementBufferGenerator(class_bufferObjects &bufferObjects){
    // Initialize first prime's element data
    bufferObjects.eboData[0] = 0;
    bufferObjects.eboData[1] = 1;      
    int inputData = 2;
    for(int i = 2; i + 2 < bufferObjects.eboElementsPerVertexRing; i += bufferObjects.eboVerticesPerTriangle){
        bufferObjects.eboData[i]     = inputData;
        bufferObjects.eboData[i + 2] = inputData;
        inputData++;
    }
    bufferObjects.eboData[bufferObjects.eboElementsPerVertexRing - 1] = 1; 
    
    // Initialize second prime's element data
    const int finalPrimeStart = bufferObjects.totalVerticesCount - bufferObjects.verticesPerRingCount;
    bufferObjects.eboData[bufferObjects.eboCapacity - bufferObjects.eboElementsPerVertexRing + 1] = finalPrimeStart - 1;
    inputData = finalPrimeStart;
    for(int i = bufferObjects.eboCapacity - bufferObjects.eboElementsPerVertexRing; i < bufferObjects.eboCapacity - bufferObjects.eboVerticesPerTriangle; i += bufferObjects.eboVerticesPerTriangle){
        bufferObjects.eboData[i]     = bufferObjects.totalVerticesCount - 1;
        bufferObjects.eboData[i + 2] = inputData;
        if(i + 6 == bufferObjects.eboCapacity){
            bufferObjects.eboData[i + 3] = bufferObjects.totalVerticesCount - 1;
            bufferObjects.eboData[i + 4] = inputData;
            bufferObjects.eboData[i + 5] = finalPrimeStart - 1;
        }else{
            bufferObjects.eboData[i + 4] = inputData;
        }
        inputData++;
    } 
    
    // Generating element buffer data for the layers
    inputData = 1;
    const int firstPhaseOffset = bufferObjects.eboElementsPerVertexRing;
    for(int currentLayer = 0; currentLayer < bufferObjects.layerCount - 1; currentLayer++){
        int index = (bufferObjects.eboElementsPerVertexRing * currentLayer) + firstPhaseOffset;
        for(int i = index; i < index + bufferObjects.eboElementsPerVertexRing; i += 3){
            bufferObjects.eboData[i]     = inputData;
            bufferObjects.eboData[i + 1] = inputData + 1;
            bufferObjects.eboData[i + 2] = inputData + bufferObjects.verticesPerRingCount;

            inputData++;
        }
        bufferObjects.eboData[index + bufferObjects.eboElementsPerVertexRing - 2] =  bufferObjects.verticesPerRingCount * currentLayer + 1;
    }
    
    inputData = 1;
    const int secondPhaseOffset = bufferObjects.eboElementsPerVertexRing + ((bufferObjects.layerCount - 1) * bufferObjects.eboElementsPerVertexRing);
    for(int currentLayer = 0; currentLayer < bufferObjects.layerCount - 1; currentLayer++){
        int index = (bufferObjects.eboElementsPerVertexRing * currentLayer)+ secondPhaseOffset;
        for(int i = index; i < index + bufferObjects.eboElementsPerVertexRing; i += 3){
            bufferObjects.eboData[i]     = inputData; 
            bufferObjects.eboData[i + 1] = inputData + bufferObjects.verticesPerRingCount - 1;
            bufferObjects.eboData[i + 2] = inputData + bufferObjects.verticesPerRingCount;

            inputData++;
        }
        bufferObjects.eboData[index + 1] = (currentLayer + 1) * bufferObjects.verticesPerRingCount + bufferObjects.verticesPerRingCount;
    }

}
*/

void debug_vboDataDisplayer(class_bufferObjects &bufferObjects){
    std::cout << "Vertices Per Ring: " << bufferObjects.verticesPerRingCount << std::endl
              << "Total Vertices:    " << bufferObjects.totalVerticesCount << std::endl
              << "Layer Count:       " << bufferObjects.layerCount << std::endl
              << "Array Capacity:    " << bufferObjects.capacity << std::endl;

    const int elementsPerLayer = bufferObjects.verticesPerRingCount * bufferObjects.stride;
    for(int element = 0; element < bufferObjects.capacity; element++){
        if(element % (elementsPerLayer) == 0){
            std::cout << "\nLayer: " << element / elementsPerLayer << std::endl;
        }

        if(element % 6 == 0){
            std::cout << std::endl << bufferObjects.vertices[element] << ", ";
        }else{
        std::cout << bufferObjects.vertices[element] << ", ";
        }
    }

    std::cout << std:: endl;
}
void debug_eboDataDisplayer(class_bufferObjects &bufferObjects){
    std::cout << "Total Triangles: " << bufferObjects.eboCapacity / bufferObjects.eboVerticesPerTriangle << std::endl
              << "Total Elements:  " << bufferObjects.eboCapacity << std::endl << std::endl;

    
    for(int i = 0; i < bufferObjects.eboCapacity; ++i){
        if(i == bufferObjects.eboElementsPerVertexRing || i == bufferObjects.eboCapacity - bufferObjects.eboElementsPerVertexRing){
            std::cout << std::endl << "PRIME LAYER: " << std::endl;
        }

        if(i % 3 == 0){
            std::cout << std::endl << i / 3 + 1 << " | " << bufferObjects.eboData[i];
        }else{
            std::cout << ", " << bufferObjects.eboData[i];
        } 
    }

    std::cout << std::endl;
}

void programInit(int &iVerticesPerRing, float &iRadius){
    bool entryChoice = false;

    std::cout << "Enter 1 to proceed with manual entry of bufferObjects properties,\n"
              << "otherwise enter 0 to proceed with te initial properties\n";
    std::cin >> entryChoice;
    if (entryChoice){
        do{
            std::cout << "Enter the number of vertices per ring, must be a power of 2 greater than 2" << std::endl;
            std::cin >> iVerticesPerRing;
        }while(iVerticesPerRing <= 2 || iVerticesPerRing > 0 && ((iVerticesPerRing & (iVerticesPerRing - 1)) != 0)); 
        
        std::cout << "Enter the radius of the bufferObjects" << std::endl;
        std::cin >> iRadius;
    }
    return;
}
