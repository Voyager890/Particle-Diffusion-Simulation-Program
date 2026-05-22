#include "glad/glad.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <cstdlib>
#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>
#include <iterator>
#include <math.h>
#include <new>
#include <random>
#include <cmath>

#include "shaders/class.hpp"

class vertexBufferObject_class {
    public:
        const unsigned int stride = 6; // Number of attributes per vertex 
      
        // Construct parameters
        unsigned int verticesPerRingCount;
        float radius;

        vertexBufferObject_class(const unsigned int verticesPerRingCount, const float radius)
        : verticesPerRingCount(verticesPerRingCount), radius(radius){}


        // Attributes derived from construct parameters
        const unsigned int layerCount = (verticesPerRingCount % 4 == 0) 
        ?(verticesPerRingCount / 2) - 1 
        : verticesPerRingCount / 2;
        const unsigned int totalVerticesCount = verticesPerRingCount * layerCount + 2;
        const unsigned int capacity = totalVerticesCount * stride; 
        const float displacementAngle = 360.0 / verticesPerRingCount; // Angle between non-diagnolly adjacent vertices
        

        float *vertices = new float [totalVerticesCount * stride]{0.0f};

        
};

int wWidth = 1280;
int wHeight = 720;

void inputCheck(GLFWwindow* window, glm::mat4& matrix);
void resize_callback(GLFWwindow* window,int width, int height);

void vertexRingGenerator(vertexBufferObject_class &sphere);

int main(){

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

    
    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    shaderProgram.use();
    glm::mat4 Identity = glm::mat4(1.0f);
    shaderProgram.setMat4("VertexTransformer", Identity);

    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)wWidth/(float)wHeight, 0.1f, 100.0f);
    shaderProgram.setMat4("Projection", ProjectionMatrix);
    
    glm::mat4 CameraInitialize = glm::mat4(1.0f);
    CameraInitialize = glm::translate(CameraInitialize, glm::vec3(0.0f, 0.0f, -3.f));
    shaderProgram.setMat4("CameraInitialize", CameraInitialize);

    vertexBufferObject_class sphere(32, 1.0f);
    

    vertexRingGenerator(sphere);    
    // debug_printVerticesData(sphere.verticesPerRingCount, sphere.stride, vertices);
    

    unsigned int vbo, vao;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.verticesPerRingCount * sphere.stride * sizeof(float), sphere.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphere.stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sphere.stride * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while(!glfwWindowShouldClose(window)){
        
        inputCheck(window, Identity);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.setMat4("VertexTransformer", Identity);

        shaderProgram.use();
        glBindVertexArray(vao);

        glDrawArrays(GL_POINTS, 0, sphere.totalVerticesCount);
        // glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

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
}

void vertexRingGenerator(vertexBufferObject_class &sphere){


    for(int step = 0; step < sphere.verticesPerRingCount; step++){
        const float yaw = glm::radians(sphere.displacementAngle * step);
        const int currentVertex = sphere.stride * step;


        sphere.vertices[currentVertex]     = cos(yaw);   // X position Attribute    
        sphere.vertices[currentVertex + 1] = sin(yaw);   // Y position Attribute
        sphere.vertices[currentVertex + 2] = 0.0f;       // Z position Attribute
        for(int i = 3; i <= sphere.stride - 1; i++){sphere.vertices[currentVertex + i] = 1.0f;} // Setting the color attribute, radians are all the same
        
        // Correcting for floating point precision error during angle to radians conversion
        sphere.vertices[currentVertex] = std::abs(sphere.vertices[currentVertex]) < 0.0001 ? 0.0f : sphere.vertices[currentVertex]; 
        sphere.vertices[currentVertex] = std::abs(sphere.vertices[currentVertex]) > 0.9999 ? std::copysign(1.0, sphere.vertices[currentVertex]) : sphere.vertices[currentVertex];

        sphere.vertices[currentVertex + 1] = std::abs(sphere.vertices[currentVertex + 1]) < 0.0001 ? 0.0f : sphere.vertices[currentVertex + 1];
        sphere.vertices[currentVertex + 1] = std::abs(sphere.vertices[currentVertex + 1]) > 0.9999 ? std::copysign(1.0, sphere.vertices[currentVertex + 1]) : sphere.vertices[currentVertex + 1];

        // Scale to sphere.radius
        sphere.vertices[currentVertex]     *= sphere.radius; // X Attrubute 
        sphere.vertices[currentVertex + 1] *= sphere.radius; // Y Attrubute
        sphere.vertices[currentVertex + 2] *= sphere.radius; // Z Attrubute
    }    
}


