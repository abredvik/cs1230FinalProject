#ifndef BEZIER_H
#define BEZIER_H

#include <iostream>
#include "glm/glm.hpp"


class Bezier
{
public:
//    Bezier();
    static glm::vec4 findCurvePoint(std::vector<glm::vec4> controlPoints, float t);
    static void testPoints();
};

#endif // BEZIER_H
