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

int wWidth = 1280;
int wHeight = 720;

void inputCheck(GLFWwindow* window, glm::mat4& matrix);
void resize_callback(GLFWwindow* window,int width, int height);

void vertexRingGenerator(const int &vertices, const int &vertexAttribCount, float *operand, const float &radius);
void debug_printVerticesData(const int &verticesCount, const int &vertexAttribCount, float *operand);
void elementBufferGenerator(const int &verticesCount, unsigned int *operand);

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

    
    const int vertexCount = 32; // EDIT TO CHANGE THE DIMENSIONS
    const float radius = 1.0;  // EDIT TO CHANGE THE SIZE
    const int vertexAttribCount = 6;

    float *vertices = new float[vertexCount * vertexAttribCount]{};
    
    vertexRingGenerator(vertexCount, vertexAttribCount, vertices, radius);
    // debug_printVerticesData(vertexCount, vertexAttribCount, vertices);
    
    unsigned int *elementBuffer = new unsigned int[(vertexCount - 1) * 3]{}; 
    elementBufferGenerator(vertexCount, elementBuffer);

    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexAttribCount * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vertexCount - 1) * 3 * sizeof(unsigned int), elementBuffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while(!glfwWindowShouldClose(window)){
        
        inputCheck(window, Identity);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.setMat4("VertexTransformer", Identity);

        shaderProgram.use();
        glBindVertexArray(vao);

        glDrawElements(GL_POINTS, (vertexCount - 1) * 3, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] vertices;
    delete[] elementBuffer;
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

void vertexRingGenerator(const int &verticesCount, const int &vertexAttribCount,float *operand, const float &radius){
    const int capacity = verticesCount * vertexAttribCount;
    const float arcLength = 360.0 /  verticesCount; 
    const int stride = capacity / verticesCount;     
    const unsigned int layerCount = (verticesCount % 4 == 0) 
    ? (verticesCount / 2) - 1 
    :  verticesCount / 2;

    for(int step = 0; step < verticesCount; step++){
        const float yaw = glm::radians(arcLength * step);
        const int currentVertex = stride * step;


        operand[currentVertex]     = cos(yaw);   // X position Attribute    
        operand[currentVertex + 1] = sin(yaw);   // Y position Attribute
        operand[currentVertex + 2] = 0.0f;       // Z position Attribute
        for(int i = 3; i <= stride - 1; i++){operand[currentVertex + i] = 1.0f;} // Setting the color attribute, radians are all the same
        
        // Correcting for floating point precision error during angle to radians conversion
        operand[currentVertex] = std::abs(operand[currentVertex]) < 0.0001 ? 0.0f : operand[currentVertex]; 
        operand[currentVertex] = std::abs(operand[currentVertex]) > 0.9999 ? std::copysign(1.0, operand[currentVertex]) : operand[currentVertex];

        operand[currentVertex + 1] = std::abs(operand[currentVertex + 1]) < 0.0001 ? 0.0f : operand[currentVertex + 1];
        operand[currentVertex + 1] = std::abs(operand[currentVertex + 1]) > 0.9999 ? std::copysign(1.0, operand[currentVertex + 1]) : operand[currentVertex + 1];

        // Scale to radius
        operand[currentVertex]     *= radius; // X Attrubute 
        operand[currentVertex + 1] *= radius; // Y Attrubute
        operand[currentVertex + 2] *= radius; // Z Attrubute
    }    
}

void elementBufferGenerator(const int &verticesCount, unsigned int *operand){
    const int capacity = (verticesCount - 1) * 3; // 3 vertices per triangle

    unsigned int vertex = 0;
    for(int i = 0; i < capacity - 1; i++){ // Leave the last element 
        operand[i] = vertex;
        vertex = ((i + 1) % 3) == 0 ? vertex -= 1 : vertex += 1;
    }
    operand[capacity] = 1; // Since it has to loop back to the first
    
    /*
    std::cout << "element buffer object population:" << std::endl;
    for(int i = 0; i < capacity; i++){
        
        std::cout << operand[i] << " , ";
    }
    std::cout << std::endl;
    */
}

void debug_printVerticesData(const int &verticesCount, const int &vertexAttribCount, float *operand){
    const int capacity = verticesCount * vertexAttribCount; // vertexAttribCount is the total attributes per vertex (3 position data, 3 color data)
    const int stride = capacity / verticesCount;
    
    std::cout << "|";
    for(int i = 0; i < capacity; i++){
        std::cout << operand[i] << ", ";
        if((i + 1) % vertexAttribCount == 0){std::cout << "|" << std::endl << "|";}
        
    }
    
}
