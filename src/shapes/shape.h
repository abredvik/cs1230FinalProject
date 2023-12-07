#pragma once

#include <vector>
#include <glm/glm.hpp>

class Shape
{
public:
    virtual void updateParams(int param1, int param2) = 0;

    virtual const std::vector<float>& generateShape() const = 0;

    virtual const int getNumVertices() const = 0;

    glm::vec3 getTang(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);

protected:
    Shape();

};
