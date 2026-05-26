#include "glad/glad.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
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
#include <cmath>

#include "shaders/class.hpp"

class class_bufferObjects {
    public:
        const unsigned int stride = 6; // Number of attributes per vertex 
      
        // Construct parameters
        unsigned int verticesPerRingCount;
        float radius;

        class_bufferObjects(const unsigned int verticesPerRingCount, const float radius)
        : verticesPerRingCount(verticesPerRingCount), radius(radius){}


        // Attributes derived from construct parameters
        const bool loneEndVertex = (verticesPerRingCount % 4 == 0) ? true : false; // Referring to the first and last layer
        const unsigned int layerCount = loneEndVertex ?(verticesPerRingCount / 2) - 1 : verticesPerRingCount / 2; // Doesnt count end vertices as layers  
        unsigned int totalVerticesCount = (verticesPerRingCount * layerCount); // A variable since it will change when appending the first and final vertex
        unsigned int capacity = totalVerticesCount * stride; 
        const float displacementAngle = 360.0 / verticesPerRingCount; // Angle between non-diagnolly adjacent vertices
        
        float *vertices = new float [capacity]{};

        // Element Buffer Object Properties
        const int eboVerticesPerTriangle = 3;
        const int eboCapacity = ((layerCount - 1) * verticesPerRingCount * 2 * eboVerticesPerTriangle) + (verticesPerRingCount * eboVerticesPerTriangle * 2);
        int *eboData = new int [eboCapacity]{};

};

int wWidth = 1280;
int wHeight = 720;

void programInit(int &iVerticesPerRing, float &iRadius);

void inputCheck(GLFWwindow* window, glm::mat4& matrix);
void resize_callback(GLFWwindow* window,int width, int height);

void vertexRingGenerator(class_bufferObjects &sphere);
void primaryVertexInit(class_bufferObjects &sphere);
void elementBufferGenerator(class_bufferObjects &sphere);

void debug_vboDataDisplayer(class_bufferObjects &sphere);
void debug_eboDataDisplayer(class_bufferObjects &sphere);

int main(){
    // Initialize the required parameters
    int iVerticesPerRing = 8;
    float iRadius = 1.0f;
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

    
    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    shaderProgram.use();
    glm::mat4 Identity = glm::mat4(1.0f);
    shaderProgram.setMat4("VertexTransformer", Identity);

    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)wWidth/(float)wHeight, 0.1f, 100.0f);
    shaderProgram.setMat4("Projection", ProjectionMatrix);
    
    glm::mat4 CameraInitialize = glm::mat4(1.0f);
    CameraInitialize = glm::translate(CameraInitialize, glm::vec3(0.0f, 0.0f, -6.f));
    shaderProgram.setMat4("CameraInitialize", CameraInitialize);

    class_bufferObjects sphere(iVerticesPerRing, iRadius);
    vertexRingGenerator(sphere);    
    elementBufferGenerator(sphere);
    debug_eboDataDisplayer(sphere);

    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.capacity * sizeof(float), sphere.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.eboCapacity * sizeof(int), sphere.eboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphere.stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sphere.stride * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wire Frame
    
    while(!glfwWindowShouldClose(window)){
        
        inputCheck(window, Identity);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.setMat4("VertexTransformer", Identity);

        shaderProgram.use();
        glBindVertexArray(vao);

        glDrawArrays(GL_POINTS, 0, sphere.totalVerticesCount);
        // glDrawElements(GL_TRIANGLES, sphere.eboCapacity, GL_INT, 0);

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

void vertexRingGenerator(class_bufferObjects &sphere){
    for(int currentLayer = 0; currentLayer < sphere.layerCount; currentLayer++){
        const float pitch = glm::radians(90.0 - ((currentLayer + 1) * sphere.displacementAngle));

        for(int step = 0; step < sphere.verticesPerRingCount; step++){
            const float yaw = glm::radians(sphere.displacementAngle * step);
            const int currentVertex = (sphere.stride * step) + (sphere.verticesPerRingCount * sphere.stride * (currentLayer));

            // Calculate and scale vertex position to fit the radius sphere
            sphere.vertices[currentVertex]     = cos(yaw)   * (cos(pitch) * sphere.radius);   // X position Attribute    
            sphere.vertices[currentVertex + 1] = sin(yaw)   * (cos(pitch) * sphere.radius);   // Y position Attribute
            sphere.vertices[currentVertex + 2] = sin(pitch) * sphere.radius;                  // Z position Attribute
            
            // Setting the color attribute, values are all the same
            for(int i = 3; i <= sphere.stride - 1; ++i){sphere.vertices[currentVertex + i] = 1.0f;} 
            
            // Correcting for floating point precision error in angle to radians conversion
            for(int i = 0; i < 3; ++i){
                sphere.vertices[currentVertex + i] = std::abs(sphere.vertices[currentVertex + i]) < 0.0001 ? 0.0f : sphere.vertices[currentVertex + i]; 
                sphere.vertices[currentVertex + i] = std::abs(sphere.vertices[currentVertex + i]) > 0.9999 * sphere.radius ? std::copysign(1.0 * sphere.radius, sphere.vertices[currentVertex + i]) : sphere.vertices[currentVertex + i];
            }
        }    
    }

    primaryVertexInit(sphere);
}

void primaryVertexInit(class_bufferObjects &sphere){
    float *newBuffer = new float[sphere.capacity + (2 * sphere.stride)]{0.0f};

    const int offset = sphere.stride * 1; // How many starting elements to offset by 
    for(int i = 0; i < sphere.capacity; ++i){newBuffer[i + offset] = sphere.vertices[i];};
    

    newBuffer[0] = 0.0f * sphere.radius;
    newBuffer[1] = 0.0f * sphere.radius;
    newBuffer[2] = 1.0f * sphere.radius;
    for(int i = 3; i < 6; ++i){newBuffer[i] = 1.0f;}
    
    const int lastVertexElementStart = sphere.capacity + sphere.stride; // The new data array size has not beenn set yet 
    newBuffer[lastVertexElementStart]     = 0.0f * sphere.radius;
    newBuffer[lastVertexElementStart + 1] = 0.0f * sphere.radius;
    newBuffer[lastVertexElementStart + 2] =-1.0f * sphere.radius;
    for(int i = 3; i < 6; ++i){
        newBuffer[lastVertexElementStart + i] = 1.0f;
    }

    sphere.totalVerticesCount += 2;
    sphere.capacity = sphere.totalVerticesCount * sphere.stride;
    delete[] sphere.vertices;
    sphere.vertices = newBuffer;
}

void elementBufferGenerator(class_bufferObjects &sphere){
    const int elementsPerVertexRing = sphere.verticesPerRingCount * sphere.eboVerticesPerTriangle;
    // Initializing first prime elements data
    sphere.eboData[0] = 0;
    sphere.eboData[1] = 1;      
    int j = 2;
    for(int i = 2; i + 2 < elementsPerVertexRing; i += sphere.eboVerticesPerTriangle){
        sphere.eboData[i]     = j;
        sphere.eboData[i + 2] = j;
        j++;
    }
    sphere.eboData[elementsPerVertexRing - 1] = 1; 
    
}

void debug_vboDataDisplayer(class_bufferObjects &sphere){
    std::cout << "Vertices Per Ring: " << sphere.verticesPerRingCount << std::endl
              << "Total Vertices:    " << sphere.totalVerticesCount << std::endl
              << "Layer Count:       " << sphere.layerCount << std::endl
              << "Array Capacity:    " << sphere.capacity << std::endl;

    const int elementsPerLayer = sphere.verticesPerRingCount * sphere.stride;
    for(int element = 0; element < sphere.capacity; element++){
        if(element % (elementsPerLayer) == 0){
            std::cout << "\nLayer: " << element / elementsPerLayer << std::endl;
        }

        if(element % 6 == 0){
            std::cout << std::endl << sphere.vertices[element] << ", ";
        }else{
        std::cout << sphere.vertices[element] << ", ";
        }
    }

    std::cout << std:: endl;
}
void debug_eboDataDisplayer(class_bufferObjects &sphere){
    std::cout << "Total Triangles: " << sphere.eboCapacity / sphere.eboVerticesPerTriangle << std::endl
              << "Total Elements:  " << sphere.eboCapacity << std::endl << std::endl;

    const int elementsPerVertexRing = sphere.verticesPerRingCount * sphere.eboVerticesPerTriangle;
    const int elementsPerLayer = sphere.verticesPerRingCount * sphere.eboVerticesPerTriangle * 2;
    for(int i = 0; i < sphere.eboCapacity; ++i){
        if(i == elementsPerVertexRing || i == sphere.eboCapacity - elementsPerVertexRing){std::cout << std::endl << "PRIME LAYER: " << std::endl;}         
        if(i % 3 == 0){
            std::cout << std::endl << sphere.eboData[i];
        }else{
            std::cout << ", " << sphere.eboData[i];
        } 
    }
}

void programInit(int &iVerticesPerRing, float &iRadius){
    bool entryChoice = false;

    std::cout << "Enter 1 to proceed with manual entry of sphere properties,\n"
              << "otherwise enter 0 to proceed with te initial properties\n";
    std::cin >> entryChoice;
    if (entryChoice){
        do{
            std::cout << "Enter the number of vertices per ring, must be a power of 2 greater than 2" << std::endl;
            std::cin >> iVerticesPerRing;
        }while(iVerticesPerRing <= 2 || iVerticesPerRing > 0 && ((iVerticesPerRing & (iVerticesPerRing - 1)) != 0)); 
        
        std::cout << "Enter the radius of the sphere" << std::endl;
        std::cin >> iRadius;
    }
    return;
}
