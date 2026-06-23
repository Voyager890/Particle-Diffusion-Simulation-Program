#include "glad/glad.h"
#include "proceduralGeometry.h"

GLuint initBorderBuffer(const double borderDisplacement){
    const float e = borderDisplacement;
    float vertexBufferObjectData[] = {
         e,  e,  e,
         e,  e, -e,
        -e,  e, -e,
        -e,  e,  e,
    
         e, -e,  e,
         e, -e, -e,
        -e, -e, -e,
        -e, -e,  e,
    };

    unsigned int order[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        4, 5,
        5, 6,
        6, 7,
        7, 4,

        0, 4,
        1, 5,
        2, 6,
        3, 7,
    };

    GLuint vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferObjectData), vertexBufferObjectData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return vao;
}