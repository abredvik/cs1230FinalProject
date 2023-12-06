#include "bezier.h"
#include "glm/fwd.hpp"
#include <iostream>

//Bezier::Bezier()
//{

//}

glm::vec4 Bezier::findCurvePoint(std::vector<glm::vec4> controlPoints, float t) {

    if (controlPoints.size() == 1) {

        return controlPoints[0];

        }

    else {
        std::vector<glm::vec4> newPoints = std::vector<glm::vec4>(controlPoints.size() - 1);

        for (int i = 0; i < newPoints.size(); i++) {

            glm::vec4 newPoint;

            newPoint = (1 - t) * controlPoints[i] + t * controlPoints[i + 1];

//            float x = (1 - t) * controlPoints[i].x + t * controlPoints[i + 1].x;
//            float y = (1 - t) * controlPoints[i].y + t * controlPoints[i + 1].y;
//            float z = (1 - t) * controlPoints[i].z + t * controlPoints[i + 1].z;

//            newPoints[i] = glm::vec4(x, y, z, 1.0f);

            newPoints[i] = newPoint;

            findCurvePoint(newPoints, t);
            }
        }
}

void Bezier::testPoints() {

        glm::vec4 controlPointA = glm::vec4(40, 100, 1, 1);
        glm::vec4 controlPointB = glm::vec4(80, 20, 1, 1);
        glm::vec4 controlPointC = glm::vec4(150, 180, 1, 1);
        glm::vec4 controlPointD = glm::vec4(260, 100, 1, 1);

        std::vector<glm::vec4> controlPoints;

        controlPoints.push_back(controlPointA);
        controlPoints.push_back(controlPointB);
        controlPoints.push_back(controlPointC);
        controlPoints.push_back(controlPointD);

        for (int i = 0; i < 100; i++) {
            float t = (float)(i)/99.0f;
            glm::vec4 pathPoint = Bezier::findCurvePoint(controlPoints, t);

            std::cout << "pathPoint.x: " << pathPoint.x << " pathPoint.y: " << pathPoint.y << "pathPoint.z: " << pathPoint.z << "pathPoint[3]: " << pathPoint[3] << std::endl;
        }
}
