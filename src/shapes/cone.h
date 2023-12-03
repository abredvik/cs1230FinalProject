#pragma once

#include "shapes/shape.h"
#include <vector>
#include <glm/glm.hpp>

class Cone : public Shape
{
public:
    Cone() { setVertexData(); num_vertices = m_vertexData.size() / 6; };
    void updateParams(int param1, int param2);
    const std::vector<float>& generateShape() const { return m_vertexData; }
    const int getNumVertices() const { return num_vertices; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();

    glm::vec3 getNormal(glm::vec3 v, bool isCap);
    void makeCone();
    void makeCap();
    void makeFace(float theta, float nextTheta, bool isCap);
    void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, bool isCap);

    std::vector<float> m_vertexData;
    int num_vertices;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
