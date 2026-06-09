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

void debug_vboDataDisplayer(class_bufferObjects &bufferObjects);
void debug_eboDataDisplayer(class_bufferObjects &bufferObjects);

int main(){
    // Initialize the required parameters
    int iVerticesPerRing = 32;
    float iRadius = 1.0f;
    glm::vec3 lightSourceOrigin(0.0f, 0.0f, 0.0f);   
    glm::vec3 lightColor(1.0f);

    programInit(iVerticesPerRing, iRadius);

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


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wire Frame
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
