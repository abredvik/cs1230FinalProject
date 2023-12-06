#include "terraingenerator.h"

#include <cmath>

// Constructor
TerrainGenerator::TerrainGenerator()
{
  // Task 8: turn off wireframe shading
//  m_wireshade = true; // STENCIL CODE
   m_wireshade = false; // TA SOLUTION

  // Define resolution of terrain generation
  m_resolution = 100;

  // Generate random vector lookup table
  m_lookupSize = 1024;
  m_randVecLookup.reserve(m_lookupSize);

  // Initialize random number generator
  std::srand(1230);

  // Populate random vector lookup table
  for (int i = 0; i < m_lookupSize; i++)
  {
    m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,
                                        std::rand() * 2.0 / RAND_MAX - 1.0));
    }
}

// Destructor
TerrainGenerator::~TerrainGenerator()
{
    m_randVecLookup.clear();
}

// Helper for generateTerrain()
void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

// Generates the geometry of the output triangle mesh
std::vector<float> TerrainGenerator::generateTerrain() {
    std::vector<float> verts;
    verts.reserve(m_resolution * m_resolution * 6);

    for(int x = 0; x < m_resolution; x++) {
        for(int y = 0; y < m_resolution; y++) {
            int x1 = x;
            int y1 = y;

            int x2 = x + 1;
            int y2 = y + 1;

            glm::vec3 p1 = getPosition(x1,y1);
            glm::vec3 p2 = getPosition(x2,y1);
            glm::vec3 p3 = getPosition(x2,y2);
            glm::vec3 p4 = getPosition(x1,y2);

            glm::vec3 n1 = getNormal(x1,y1);
            glm::vec3 n2 = getNormal(x2,y1);
            glm::vec3 n3 = getNormal(x2,y2);
            glm::vec3 n4 = getNormal(x1,y2);

            // tris 1
            // x1y1z1
            // x2y1z2
            // x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
//            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
//            addPointToVector(getColor(n2, p2), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
//            addPointToVector(getColor(n3, p3), verts);

            // tris 2
            // x1y1z1
            // x2y2z3
            // x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
//            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
//            addPointToVector(getColor(n3, p3), verts);

            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
//            addPointToVector(getColor(n4, p4), verts);
        }
    }
    return verts;
}

// Samples the (infinite) random vector grid at (row, col)
glm::vec2 TerrainGenerator::sampleRandomVector(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Takes a grid coordinate (row, col), [0, m_resolution), which describes a vertex in a plane mesh
// Returns a normalized position (x, y, z); x and y in range from [0, 1), and z is obtained from getHeight()
glm::vec3 TerrainGenerator::getPosition(int row, int col) {
    // Normalizing the planar coordinates to a unit square 
    // makes scaling independent of sampling resolution.
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;
    float z = getHeight(x, y);
    return glm::vec3(x,y,z);
}

// ================== Students, please focus on the code below this point

// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    // Task 4: implement your easing/interpolation function below
    float x2 = alpha * alpha;
    float x3 = x2 * alpha;
    float ease = 3.f * x2 - 2.f * x3;

    // Return 0 as placeholder
    return A + ease * (B - A);
}

// Takes a normalized (x, y) position, in range [0,1)
// Returns a height value, z, by sampling a noise function
float TerrainGenerator::getHeight(float x, float y) {

    // Task 6: modify this call to produce noise of a different frequency
    float z = computePerlin(x * 2, y * 2) / 2;

    // Task 7: combine multiple different octaves of noise to produce fractal perlin noise
    z += computePerlin(x * 4, y * 4) / 4;
    z += computePerlin(x * 8, y * 8) / 8;
    z += computePerlin(x * 16, y * 16) / 16;
    z += computePerlin(x * 32, y * 32) / 32;

    // Return 0 as placeholder
    return z;
}

// Computes the normal of a vertex by averaging neighbors
glm::vec3 TerrainGenerator::getNormal(int row, int col) {
    // Task 9: Compute the average normal for the given input indices
    glm::vec3 V = getPosition(row, col);
    std::vector<glm::vec3> n(8);
    n[0] = getPosition(row - 1, col - 1);
    n[1] = getPosition(row, col - 1);
    n[2] = getPosition(row + 1, col - 1);
    n[3] = getPosition(row + 1, col);
    n[4] = getPosition(row + 1, col + 1);
    n[5] = getPosition(row, col + 1);
    n[6] = getPosition(row - 1, col + 1);
    n[7] = getPosition(row - 1, col);

    glm::vec3 normal(0.f);
    for (int i = 0; i < 8; ++i) {
        if (i < 7)
            normal += glm::normalize(glm::cross(n[i] - V, n[i+1] - V));
        else
            normal += glm::normalize(glm::cross(n[i] - V, n[0] - V));
    }

    // Return up as placeholder
    return glm::normalize(normal);
}

// Computes color of vertex using normal and, optionally, position
glm::vec3 TerrainGenerator::getColor(glm::vec3 normal, glm::vec3 position) {
    // Task 10: compute color as a function of the normal and position
//    return (position[2] > 0.1) ? glm::vec3(1,1,1) : glm::vec3(0.5,0.5,0.5);
//    return (glm::dot(normal, glm::vec3(0,0,1)) > 0.6) ? glm::vec3(1,1,1) : glm::vec3(0.5,0.5,0.5);
    if (position[2] > 0.05 && glm::dot(normal, glm::vec3(0,0,1)) > 0.6)
        return glm::vec3(1,1,1);
    else
        return glm::vec3(0.5,0.5,0.5);

    // Return white as placeholder
//    return glm::vec3(1,1,1);
}

// Computes the intensity of Perlin noise at some point
float TerrainGenerator::computePerlin(float x, float y) {
    // Task 1: get grid indices (as ints)
    glm::vec2 coords(x, y);
    glm::vec2 topLeft(glm::floor(coords));
    glm::vec2 topRight(topLeft[0] + 1, topLeft[1]);
    glm::vec2 botLeft(topLeft[0], topLeft[1] + 1);
    glm::vec2 botRight(topLeft[0] + 1, topLeft[1] + 1);

    // Task 2: compute offset vectors
    glm::vec2 topLeftOff(coords - topLeft);
    glm::vec2 topRightOff(coords - topRight);
    glm::vec2 botLeftOff(coords - botLeft);
    glm::vec2 botRightOff(coords - botRight);

    // Task 3: compute the dot product between the grid point direction vectors and its offset vectors
    float A = glm::dot(topLeftOff, sampleRandomVector(topLeft[0], topLeft[1])); // dot product between top-left direction and its offset
    float B = glm::dot(topRightOff, sampleRandomVector(topRight[0], topRight[1]));  // dot product between top-right direction and its offset
    float C = glm::dot(botLeftOff, sampleRandomVector(botLeft[0], botLeft[1])); // dot product between bottom-right direction and its offset
    float D = glm::dot(botRightOff, sampleRandomVector(botRight[0], botRight[1])); // dot product between bottom-left direction and its offset

    // Task 5: Debug this line to properly use your interpolation function to produce the correct value
    return interpolate(interpolate(A, B, x - topLeft[0]), interpolate(C, D, x - topLeft[0]), y - topLeft[1]);
}
