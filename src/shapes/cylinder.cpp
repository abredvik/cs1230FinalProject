#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = std::max(2, param2);
    setVertexData();
    num_vertices = m_vertexData.size() / 9;
}

glm::vec3 Cylinder::getNormal(glm::vec3 v, bool isCap) {
    if (isCap) {
        return glm::normalize(glm::vec3(0.f, v[1], 0.f));
    } else {
        return glm::normalize(glm::vec3(2.f * v[0], 0.f, 2.f * v[2]));
    }
}

void Cylinder::makeTile(glm::vec3 topLeft,
                        glm::vec3 topRight,
                        glm::vec3 bottomLeft,
                        glm::vec3 bottomRight,
                        bool isCap) {

    // left triangle
    glm::vec3 tan1 = getTang(topLeft, bottomLeft, bottomRight);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, getNormal(topLeft, isCap));
    insertVec3(m_vertexData, tan1);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, getNormal(bottomLeft, isCap));
    insertVec3(m_vertexData, tan1);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, getNormal(bottomRight, isCap));
    insertVec3(m_vertexData, tan1);

    // right triangle
    glm::vec3 tan2 = getTang(topLeft, bottomRight, topRight);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, getNormal(topLeft, isCap));
    insertVec3(m_vertexData, tan2);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, getNormal(bottomRight, isCap));
    insertVec3(m_vertexData, tan2);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, getNormal(topRight, isCap));
    insertVec3(m_vertexData, tan2);
}

void Cylinder::makeCap(float theta, float thetaStep, bool isTop) {
    // outermost vertices
    float nextTheta = theta + thetaStep;
    glm::vec3 v0, v1, v2, normal;
    if (isTop) {
        normal = glm::vec3(0.f, 1.f, 0.f);
        v0 = 0.5f * glm::vec3(0.f, 1.f, 0.f);
        v1 = 0.5f * glm::vec3(glm::sin(theta), 1.f, glm::cos(theta));
        v2 = 0.5f * glm::vec3(glm::sin(nextTheta), 1.f, glm::cos(nextTheta));
    } else {
        normal = glm::vec3(0.f, -1.f, 0.f);
        v0 = 0.5f * glm::vec3(0.f, -1.f, 0.f);
        v1 = 0.5f * glm::vec3(glm::sin(nextTheta), -1.f, glm::cos(nextTheta));
        v2 = 0.5f * glm::vec3(glm::sin(theta), -1.f, glm::cos(theta));
    }

    // left edge
    glm::vec3 left = (v1 - v0) / (float)m_param1;

    // right edge
    glm::vec3 right = (v2 - v0) / (float)m_param1;

    glm::vec3 tan = getTang(v0, v0 + left, v0 + right);

    // create upper triangle
    insertVec3(m_vertexData, v0);
    insertVec3(m_vertexData, normal);
    insertVec3(m_vertexData, tan);
    insertVec3(m_vertexData, v0 + left);
    insertVec3(m_vertexData, normal);
    insertVec3(m_vertexData, tan);
    insertVec3(m_vertexData, v0 + right);
    insertVec3(m_vertexData, normal);
    insertVec3(m_vertexData, tan);

    // create lower tiles
    for (int i = 1; i < m_param1; ++i) {
        makeTile(v0 + (float)i * left,
                 v0 + (float)i * right,
                 v0 + (float)(i+1) * left,
                 v0 + (float)(i+1) * right,
                 true);
    }
}

void Cylinder::makeFace(float theta, float thetaStep) {

    float nextTheta = theta + thetaStep;
    glm::vec3 topLeft = 0.5f * glm::vec3(glm::sin(theta), 1.f, glm::cos(theta));
    glm::vec3 topRight = 0.5f * glm::vec3(glm::sin(nextTheta), 1.f, glm::cos(nextTheta));
    glm::vec3 down = glm::vec3(0.f, -1.f, 0.f) / (float)m_param1;

    for (int i = 0; i < m_param1; ++i) {
        makeTile(topLeft + (float)i * down,
                 topRight + (float)i * down,
                 topLeft + (float)(i+1) * down,
                 topRight + (float)(i+1) * down,
                 false);
    }
}

void Cylinder::makeCylinder() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; ++i) {
        float theta = i * thetaStep;
        makeCap(theta, thetaStep, false);
        makeCap(theta, thetaStep, true);
        makeFace(theta, thetaStep);
    }
}

void Cylinder::setVertexData() {
    // TODO for Project 5: Lights, Camera
    makeCylinder();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
