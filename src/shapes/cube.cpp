#include "Cube.h"

void Cube::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
    num_vertices = m_vertexData.size() / 9;
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    glm::vec3 leftNorm = glm::normalize(glm::cross(bottomLeft - topLeft, bottomRight - topLeft));
    glm::vec3 rightNorm = glm::normalize(glm::cross(bottomRight - topLeft, topRight - topLeft));

    // left triangle
    glm::vec3 tan1 = getTang(topLeft, bottomLeft, bottomRight);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, leftNorm);
    insertVec3(m_vertexData, tan1);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, leftNorm);
    insertVec3(m_vertexData, tan1);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, leftNorm);
    insertVec3(m_vertexData, tan1);

    glm::vec3 tan2 = getTang(topLeft, bottomRight, topRight);

    // right triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, rightNorm);
    insertVec3(m_vertexData, tan2);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, rightNorm);
    insertVec3(m_vertexData, tan2);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, rightNorm);
    insertVec3(m_vertexData, tan2);
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube

    glm::vec3 right = (topRight - topLeft) / (float)m_param1;
    glm::vec3 down = (bottomLeft - topLeft) / (float)m_param1;

    glm::vec3 tl = topLeft;
    glm::vec3 tr = tl + right;
    glm::vec3 bl = tl + down;
    glm::vec3 br = tl + right + down;

    glm::vec3 shift;
    for (int i = 0; i < m_param1; ++i) {
        for (int j = 0; j < m_param1; ++j) {
            shift = (float)i * right + (float)j * down;
            makeTile(tl + shift,
                     tr + shift,
                     bl + shift,
                     br + shift);
        }
    }
}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

    //     makeTile(glm::vec3(-0.5f,  0.5f, 0.5f),
    //              glm::vec3( 0.5f,  0.5f, 0.5f),
    //              glm::vec3(-0.5f, -0.5f, 0.5f),
    //              glm::vec3( 0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:

    glm::vec3 tl(-0.5f,  0.5f, 0.5f);
    glm::vec3 tr( 0.5f,  0.5f, 0.5f);
    glm::vec3 bl(-0.5f, -0.5f, 0.5f);
    glm::vec3 br( 0.5f, -0.5f, 0.5f);

    glm::vec3 x(1.0, 0.0, 0.0);
    glm::vec3 y(0.0, 1.0, 0.0);
    glm::vec3 z(0.0, 0.0, 1.0);

    // Task 4: Use the makeFace() function to make all 6 sides of the cube
    // front
    makeFace(tl, tr, bl, br);

    // top
    makeFace(tl - z, tr - z, bl + y, br + y);

    // left
    makeFace(tl - z, tr - x, bl - z, br - x);

    // right
    makeFace(tl + x, tr - z, bl + x, br - z);

    // bottom
    makeFace(tl - y, tr - y, bl - z, br - z);

    // back
    makeFace(tl + x - z, tr - x - z, bl + x - z, br - x - z);
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
