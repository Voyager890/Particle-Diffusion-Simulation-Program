#ifndef BUFFER_INIT_OBJECTS
#define BUFFER_INIT_OBJECTS

/*
    A class that helps in the vertex buffer object data generation
    by providing some constants that describe properties the sphere has
    all under one class, keeping this organized and readable.
*/
class class_bufferObjectsInitHelper {
    public:
        const unsigned int stride; // Number of attributes per vertex 

        // Construct parameters
        unsigned int verticesPerRingCount;
        float radius;

        class_bufferObjectsInitHelper(const unsigned int verticesPerRingCount, const float radius);


        // Attributes derived from construct parameters
        const unsigned int layerCount;
        unsigned int totalVerticesCount;
        unsigned int capacity;
        const float displacementAngle;
        
        float *vboData;

        // Element Buffer Object Properties
        const int eboVerticesPerTriangle;
        const int eboCapacity;
        const int eboElementsPerVertexRing;

        unsigned int *eboData;

        ~class_bufferObjectsInitHelper();

};
#endif