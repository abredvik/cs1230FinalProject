#ifndef BEZIER_H
#define BEZIER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class Bezier
{
public:
    Bezier();
    static void linearInterpolation(glm::vec3& dest, const glm::vec3& a, const glm::vec3& b, const float t);
    static void findCurvePoint(glm::vec4& dest, std::vector<glm::vec4>& controlPoints, float t);
//    static void findCurvePoint(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3 c, const glm::vec3& d, const float t);
    static std::vector<glm::vec4> testPoints();
};

#endif // BEZIER_H
