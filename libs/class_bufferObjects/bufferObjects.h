#ifndef BUFFER_OBJECTS
#define BUFFER_OBJECTS

class class_bufferObjects {
    public:
        const unsigned int stride; // Number of attributes per vertex 

        // Construct parameters
        unsigned int verticesPerRingCount;
        float radius;

        class_bufferObjects(const unsigned int verticesPerRingCount, const float radius);


        // Attributes derived from construct parameters
        const unsigned int layerCount;
        unsigned int totalVerticesCount;
        unsigned int capacity;
        const float displacementAngle;
        
        float *vertices;

        // Element Buffer Object Properties
        const int eboVerticesPerTriangle;
        const int eboCapacity;
        const int eboElementsPerVertexRing;

        unsigned int *eboData;

        ~class_bufferObjects();

};
#endif