#ifndef BEZIER_H
#define BEZIER_H

#include <iostream>
#include "glm/glm.hpp"


class Bezier
{
public:
    Bezier();
    void drawCurvePoint(std::vector<glm::vec4> points, float t);
};

#endif // BEZIER_H
