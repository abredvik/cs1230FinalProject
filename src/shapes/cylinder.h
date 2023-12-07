#pragma once

#include "shapes/shape.h"
#include <vector>

class Cylinder : public Shape
{
public:
    Cylinder() { setVertexData(); num_vertices = m_vertexData.size() / 9; };
    void updateParams(int param1, int param2);
    const std::vector<float>& generateShape() const { return m_vertexData; }
    const int getNumVertices() const { return num_vertices; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();

    glm::vec3 getNormal(glm::vec3 v, bool isCap);
    void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,  glm::vec3 bottomRight,  bool isCap);
    void makeCap(float theta, float thetaStep, bool isTop);
    void makeFace(float theta, float thetaStep);
    void makeCylinder();

    std::vector<float> m_vertexData;
    int num_vertices;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
