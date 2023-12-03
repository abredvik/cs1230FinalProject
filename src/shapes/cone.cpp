#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = std::max(2, param2);
    setVertexData();
    num_vertices = m_vertexData.size() / 6;
}

glm::vec3 Cone::getNormal(glm::vec3 v, bool isCap) {
    static const glm::vec3 down(0.f, -1.f, 0.f);
    if (isCap) {
        return down;
    } else {
        return glm::normalize(glm::vec3(8.f * v[0], 1.f - 2.f * v[1], 8.f * v[2]));
    }
}

void Cone::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    bool isCap) {

    // left triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, getNormal(topLeft, isCap));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, getNormal(bottomLeft, isCap));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, getNormal(bottomRight, isCap));

    // right triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, getNormal(topLeft, isCap));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, getNormal(bottomRight, isCap));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, getNormal(topRight, isCap));
}

void Cone::makeFace(float theta, float thetaStep, bool isCap) {
    float midTheta = theta + (thetaStep / 2.f);
    float nextTheta = theta + thetaStep;

    // outermost vertices
    glm::vec3 v0, v1, v2;
    if (isCap) {
        v0 = 0.5f * glm::vec3(0.f, -1.f, 0.f);
        v1 = 0.5f * glm::vec3(glm::sin(nextTheta), -1.f, glm::cos(nextTheta));
        v2 = 0.5f * glm::vec3(glm::sin(theta), -1.f, glm::cos(theta));
    } else {
        v0 = 0.5f * glm::vec3(0.f, 1.f, 0.f);
        v1 = 0.5f * glm::vec3(glm::sin(theta), -1.f, glm::cos(theta));
        v2 = 0.5f * glm::vec3(glm::sin(nextTheta), -1.f, glm::cos(nextTheta));
    }

    // left edge
    glm::vec3 left = (v1 - v0) / (float)m_param1;

    // right edge
    glm::vec3 right = (v2 - v0) / (float)m_param1;

    // create upper triangle
    insertVec3(m_vertexData, v0);
    if (isCap) {
        insertVec3(m_vertexData, getNormal(v0, true));
    } else {
        insertVec3(m_vertexData, getNormal(0.5f * glm::vec3(glm::sin(midTheta), -1.f, glm::cos(midTheta)), false));
    }
    insertVec3(m_vertexData, v0 + left);
    insertVec3(m_vertexData, getNormal(v0 + left, isCap));
    insertVec3(m_vertexData, v0 + right);
    insertVec3(m_vertexData, getNormal(v0 + right, isCap));

    // create lower tiles
    for (int i = 1; i < m_param1; ++i) {
        makeTile(v0 + (float)i * left,
                 v0 + (float)i * right,
                 v0 + (float)(i+1) * left,
                 v0 + (float)(i+1) * right,
                 isCap);
    }
}

void Cone::makeCone() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; ++i) {
        makeFace(i*thetaStep, thetaStep, true);
        makeFace(i*thetaStep, thetaStep, false);
    }
}


void Cone::setVertexData() {
    // TODO for Project 5: Lights, Camera
    makeCone();
}


// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
