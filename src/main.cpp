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
#include <math.h>
#include <new>
#include <cmath>

#include "shaders/class.hpp"

class class_vertexBufferObject {
    public:
        const unsigned int stride = 6; // Number of attributes per vertex 
      
        // Construct parameters
        unsigned int verticesPerRingCount;
        float radius;

        class_vertexBufferObject(const unsigned int verticesPerRingCount, const float radius)
        : verticesPerRingCount(verticesPerRingCount), radius(radius){}


        // Attributes derived from construct parameters
        const bool loneEndVertex = (verticesPerRingCount % 4 == 0) ? true : false; // Referring to the first and last layer
        const unsigned int layerCount = loneEndVertex ?(verticesPerRingCount / 2) - 1 : verticesPerRingCount / 2; // Doesnt count end vertices as layers  
        unsigned int totalVerticesCount = (verticesPerRingCount * layerCount); // A variable since it will change when appending the first and final vertex
        const unsigned int capacity = totalVerticesCount * stride; 
        const float displacementAngle = 360.0 / verticesPerRingCount; // Angle between non-diagnolly adjacent vertices
        

        float *vertices = new float [capacity]{};

        
};

int wWidth = 1280;
int wHeight = 720;

void inputCheck(GLFWwindow* window, glm::mat4& matrix);
void resize_callback(GLFWwindow* window,int width, int height);

void vertexRingGenerator(class_vertexBufferObject &sphere);
void primaryVertexInit(class_vertexBufferObject &sphere);
void debug_vboDataDisplayer(class_vertexBufferObject &sphere);

int main(){
    // Initialize the required parameters
    int iVerticesPerRing = 8;
    float iRadius = 1.0f;
    bool entryChoice;

    std::cout << "Enter a non-zero number to proceed with manual entry of sphere properties,\n"
              << "otherwise enter 0 to proceed with te initial properties\n";
    std::cin >> entryChoice;
    if (entryChoice) {
        std::cout << "Enter the number of vertices per ring" << std::endl;
        std::cin >> iVerticesPerRing;
        std::cout << "Enter the radius of the sphere" << std::endl;
        std::cin >> iRadius;
    }
    

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
    CameraInitialize = glm::translate(CameraInitialize, glm::vec3(0.0f, 0.0f, -6.f));
    shaderProgram.setMat4("CameraInitialize", CameraInitialize);

    class_vertexBufferObject sphere(iVerticesPerRing, iRadius);
    vertexRingGenerator(sphere);    
    debug_vboDataDisplayer(sphere);

    unsigned int vbo, vao;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.totalVerticesCount * sphere.stride * sizeof(float), sphere.vertices, GL_STATIC_DRAW);

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

void vertexRingGenerator(class_vertexBufferObject &sphere){
    

    for(int currentLayer = 0; currentLayer < sphere.layerCount; currentLayer++){
        const float pitch = glm::radians(90.0 - (currentLayer * sphere.displacementAngle));

        for(int step = 0; step < sphere.verticesPerRingCount; step++){
            const float yaw = glm::radians(sphere.displacementAngle * step);
            const int currentVertex = (sphere.stride * step) + (sphere.verticesPerRingCount * sphere.stride * currentLayer);

            // Calculate and scale vertex position to fit the radius sphere
            sphere.vertices[currentVertex]     = cos(yaw)   * (cos(pitch) * sphere.radius);   // X position Attribute    
            sphere.vertices[currentVertex + 1] = sin(yaw)   * (cos(pitch) * sphere.radius);   // Y position Attribute
            sphere.vertices[currentVertex + 2] = sin(pitch) * sphere.radius;   // Z position Attribute
            
            // Setting the color attribute, values are all the same
            for(int i = 3; i <= sphere.stride - 1; i++){sphere.vertices[currentVertex + i] = 1.0f;} 
            
            // Correcting for floating point precision error in angle to radians conversion
            for(int i = 0; i < 3; i++){
                sphere.vertices[currentVertex + i] = std::abs(sphere.vertices[currentVertex + i]) < 0.0001 ? 0.0f : sphere.vertices[currentVertex + i]; 
                sphere.vertices[currentVertex + i] = std::abs(sphere.vertices[currentVertex + i]) > 0.9999 ? std::copysign(1.0, sphere.vertices[currentVertex + i]) : sphere.vertices[currentVertex + i];
            }
        }    
    }
}

void primaryVertexInit(class_vertexBufferObject &sphere){}

void debug_vboDataDisplayer(class_vertexBufferObject &sphere){
    std::cout << "Vertices Per Ring: " << sphere.verticesPerRingCount << std::endl
              << "Total Vertices: " << sphere.totalVerticesCount << std::endl
              << "Layer Count: " << sphere.layerCount << std::endl
              << "Array Capacity: " << sphere.capacity << std::endl;

    const int elementsPerLayer = sphere.verticesPerRingCount * sphere.stride;
    for(int element = 0; element < sphere.capacity; element++){
        if(element % (elementsPerLayer) == 0){
            // std::cout << "\nLayer: " << element / elementsPerLayer << std::endl;
        }

        if(element % 6 == 0){
            std::cout << std::endl << sphere.vertices[element] << ", ";
        }else{
        std::cout << sphere.vertices[element] << ", ";
        }
    }

    std::cout << std:: endl;
}
