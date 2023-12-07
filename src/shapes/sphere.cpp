#include "Sphere.h"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(2, param1);
    m_param2 = std::max(2, param2);
    setVertexData();
    num_vertices = m_vertexData.size() / 9;
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!

    // left triangle
    glm::vec3 tan1 = getTang(topLeft, bottomLeft, bottomRight);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, tan1);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, tan1);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, tan1);

    // right triangle
    glm::vec3 tan2 = getTang(topLeft, bottomRight, topRight);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, tan2);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, tan2);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, tan2);

}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!

    std::vector<float> sinPhis;
    std::vector<float> cosPhis;

    float phiStep = glm::radians(180.f / (float)m_param1);
    for (int i = 0; i <= m_param1; ++i) {
        float phi = i * phiStep;
        sinPhis.push_back(glm::sin(phi));
        cosPhis.push_back(glm::cos(phi));
    }

    float sinCTheta = glm::sin(currentTheta);
    float cosCTheta = glm::cos(currentTheta);
    float sinNTheta = glm::sin(nextTheta);
    float cosNTheta = glm::cos(nextTheta);

    for (int i = 0; i < m_param1; ++i) {
        glm::vec3 tl(0.5 * sinPhis[i] * sinCTheta,
                     0.5 * cosPhis[i],
                     0.5 * sinPhis[i] * cosCTheta);
        glm::vec3 tr(0.5 * sinPhis[i] * sinNTheta,
                     0.5 * cosPhis[i],
                     0.5 * sinPhis[i] * cosNTheta);
        glm::vec3 bl(0.5 * sinPhis[i+1] * sinCTheta,
                     0.5 * cosPhis[i+1],
                     0.5 * sinPhis[i+1] * cosCTheta);
        glm::vec3 br(0.5 * sinPhis[i+1] * sinNTheta,
                     0.5 * cosPhis[i+1],
                     0.5 * sinPhis[i+1] * cosNTheta);

        makeTile(tl, tr, bl, br);
    }
}

void Sphere::makeSphere() {
    // Task 7: create a full sphere using the makeWedge() function you
    //         implemented in Task 6
    // Note: think about how param 2 comes into play here!

    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; ++i) {
        makeWedge(i * thetaStep, (i + 1) * thetaStep);
    }
}

void Sphere::setVertexData() {
    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

    //     float thetaStep = glm::radians(360.f / m_param2);
    //     float currentTheta = 0 * thetaStep;
    //     float nextTheta = 1 * thetaStep;
    //     makeWedge(currentTheta, nextTheta);

    // Uncomment these lines to make sphere for Task 7:

    makeSphere();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
