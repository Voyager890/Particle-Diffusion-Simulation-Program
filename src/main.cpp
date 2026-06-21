// External Libraris
#include "glad/glad.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <glm/common.hpp>
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

#include <new>
#include <ostream>

// Custom Libraries
#include "proceduralGeometry/proceduralBoundry.h"
#include "shaders/class.hpp"
#include "class_bufferObjectsInitHelper/bufferObjectsInitHelper.h"
#include "class_particles/particles.h"
#include "class_particleInitHelper/particleInitHelper.h"
#include "func_programStartMenu/programStartMenu.h"
#include "func_physicsEngine/physicsEngine.h"
#include "proceduralGeometry/proceduralSphere.h"

#include "debug_tools/debug_tools.h"

int wWidth = 1280;
int wHeight = 720;

void inputCheck(GLFWwindow* window, glm::mat4& matrix);
void resize_callback(GLFWwindow* window,int width, int height);

int main(){
    // Initialize the required parameters
    int iVerticesPerRing = 32;
    float borderArea = 64;
    
    class_particleInitHelper* particleInitHelper = nullptr;
    int particleTypesAmount = programInit(particleInitHelper); // Sends user to programStartMenu
    if(particleInitHelper == nullptr){std::cout << "Failed to initialize particleInitHelper object inside programStartMenu\n";return -1;}
    
    glm::vec3 lightSourceOrigin(0.0f, 0.0f, 0.0f);   
    glm::vec3 lightColor(1.0f);

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
    
    
    // Particle class initialization

    class_particleType** particleTypePointer = nullptr;
    particleTypePointer = new class_particleType*[2];
    for(int i = 0; i < particleTypesAmount; i++){
        particleTypePointer[i] = new class_particleType(particleInitHelper->name[i], particleInitHelper->color[i], particleInitHelper->mass[i], particleInitHelper->radius[i], particleInitHelper->particleCount[i]);
        if(particleTypePointer[i] == nullptr){std::cout << i << " Particle type pointer is a nullptr\n";}
    }
    
    // Shaders Initialization
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)wWidth/(float)wHeight, 0.1f, 100.0f);
    glm::mat4 cameraInit = glm::mat4(1.0f);
    glm::vec3 cameraPosition(0.0f, 0.0f, -6.0f);
    cameraInit = glm::translate(cameraInit, cameraPosition);
    glm::mat4 positionMatrix = glm::mat4(1.0f);

    Shader shader_standarad(SHADER_PATH"/vertex.glsl", SHADER_PATH"/fragment.glsl");    
    shader_standarad.use();
    shader_standarad.setMat4("motion", positionMatrix);
    shader_standarad.setMat4("camera", cameraInit);
    shader_standarad.setMat4("projection", projectionMatrix);

    shader_standarad.setVec3("objectColor", particleTypePointer[0]->objectColor);
    shader_standarad.setVec3("lightColor", lightColor);
    shader_standarad.setVec3("lightPos", lightSourceOrigin);
    
    Shader shader_lightSource(SHADER_PATH"/lightSourceVertex.glsl",SHADER_PATH"/lightSourceFragment.glsl");
    shader_lightSource.use();
    glm::mat4 lightSourceMotion(1.0f);
    lightSourceMotion = glm::translate(lightSourceMotion, lightSourceOrigin);
    shader_lightSource.setMat4("motion", lightSourceMotion);
    shader_lightSource.setMat4("camera", cameraInit);
    shader_lightSource.setMat4("projection", projectionMatrix);


    // Light source buffer object initialzation
    float lightSourceVertices[] = {
         0.0f,  1.0f, 0.0f,
         1.0f, -0.5f, 0.0f,
        -1.0f, -0.5f, 0.0f
    };
    unsigned int lightSourceVBO, lightSourceVAO;

    glGenVertexArrays(1, &lightSourceVAO);
    glGenBuffers(1, &lightSourceVBO);

    glBindVertexArray(lightSourceVAO);
    glBindBuffer(1, lightSourceVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(lightSourceVertices) / sizeof(float), lightSourceVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Particle types buffer objects initalization (ebo is shared)

    class_bufferObjectsInitHelper bufferObjectsInitHelper(iVerticesPerRing, particleTypePointer[0]->particleRadius);
    vertexRingGenerator(bufferObjectsInitHelper);

    unsigned int ebo;
    for(int i = 0; i < particleTypesAmount; i++){
        class_bufferObjectsInitHelper bufferObjects(iVerticesPerRing, particleTypePointer[i]->particleRadius);
        vertexRingGenerator(bufferObjects);

            if(i == 0){ 
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferObjects.eboCapacity * sizeof(unsigned int), bufferObjects.eboData, GL_STATIC_DRAW);
            }

        glGenBuffers(1, &particleTypePointer[i]->vertexBufferObject);
        glGenVertexArrays(1, &particleTypePointer[i]->vertexArrayObject);
        
        glBindVertexArray(particleTypePointer[i]->vertexArrayObject);

        glBindBuffer(GL_ARRAY_BUFFER, particleTypePointer[i]->vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, bufferObjects.capacity * sizeof(float), bufferObjects.vboData, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    // Border Cube buffer object init
    GLuint borderVertexArrayObject = initBorderBuffer((std::cbrt(borderArea)/2.0));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wire Frame
    glEnable(GL_DEPTH_TEST);
    
    while(!glfwWindowShouldClose(window)){
        int current = 0; 
        const float speedScaler = 0.05;
        
        inputCheck(window, positionMatrix);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        physicsEngine(particleTypePointer, particleTypesAmount, borderArea);
        shader_standarad.use();
        for(int j = 0; j < particleTypesAmount; j++){
            glBindVertexArray(particleTypePointer[j]->vertexArrayObject);
            shader_standarad.setVec3("objectColor", particleTypePointer[j]->objectColor);

            for(int i = 0; i < particleTypePointer[j]->particleCount; i++){
                particleTypePointer[j]->particle[i].position += particleTypePointer[j]->particle[i].velocity * speedScaler;

                positionMatrix = glm::mat4(1.0f);
                positionMatrix = glm::translate(positionMatrix, particleTypePointer[j]->particle[i].position);
                shader_standarad.setMat4("motion", positionMatrix);
                glDrawElements(GL_TRIANGLES, bufferObjectsInitHelper.eboCapacity, GL_UNSIGNED_INT, 0);

            }
        }
        

        shader_lightSource.use();
        glBindVertexArray(lightSourceVAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindVertexArray(borderVertexArrayObject);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete [] particleTypePointer;

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


