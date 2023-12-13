#include "bezier.h"
#include "glm/fwd.hpp"
#include <iostream>
#include <vector>

//Bezier::Bezier()
//{

//}

void Bezier::findCurvePoint(glm::vec4& dest, std::vector<glm::vec4>& controlPoints, float t) {

    if (controlPoints.size() == 1) {

//        std::cout << "X: " << controlPoints[0].x << std::endl;

        dest =  controlPoints[0];

    }

    else {
        std::vector<glm::vec4> newPoints = std::vector<glm::vec4>(controlPoints.size() - 1);

        for (int i = 0; i < newPoints.size(); i++) {

            glm::vec4 newPoint;

            newPoint = (1 - t) * controlPoints[i] + t * controlPoints[i + 1];


            newPoints[i] = newPoint;

            findCurvePoint(dest, newPoints, t);
        }
    }
}


std::vector<glm::vec4> Bezier::testPoints() {

    std::vector<glm::vec4> controlPoints = {
        glm::vec4(-3.12691f, 32.f, 20.f, 1.f),
        glm::vec4(15.f, 5.f, -20.f, 1.f),
        glm::vec4(30.f, 8.f, -40.f, 1.f),
        glm::vec4(45.f, 0.f, -60.f, 1.f),
        glm::vec4(75.f, 8.f, -75.f, 1.f),
        glm::vec4(100.f, 12.f, -90.f, 1.f),
        glm::vec4(120.f, 9.f, -110.f, 1.f),
        glm::vec4(140.f, 7.f, -140.f, 1.f),

    };

    std::vector<glm::vec4> controlPoints2 = {
        glm::vec4(160, 8, -145, 1),
        glm::vec4(180, 10, -160, 1),
        glm::vec4(200, 7, -180, 1),
        glm::vec4(225, 8, -200, 1),
        glm::vec4(245, 8, -225, 1),
        glm::vec4(260, 10, -245, 1),
        glm::vec4(280, 7, -265, 1),
        glm::vec4(300, 8, -280, 1),

    };

    std::vector<glm::vec4> controlPoints3 = {
        glm::vec4(300, 8, -280, 1),
        glm::vec4(280, 10, -265, 1),
        glm::vec4(260, 7, -245, 1),
        glm::vec4(245, 0, -225, 1),
        glm::vec4(225, 40, -200, 1),
        glm::vec4(200, 25, -180, 1),
        glm::vec4(180, 20, -160, 1),
        glm::vec4(160, 15, -145, 1),

    };

    std::vector<glm::vec4> pathPoints;

    for (int i = 400; i < 2000; i++) {
        float t = static_cast<float>(i)/1999.0f;

        glm::vec4 point;
        Bezier::findCurvePoint(point, controlPoints3, t);

        pathPoints.push_back(glm::vec4(point));

    }

    for (int i = 200; i < 1500; i++) {
        float t = static_cast<float>(i)/1999.0f;

        glm::vec4 point;
        Bezier::findCurvePoint(point, controlPoints2, t);

        pathPoints.push_back(glm::vec4(point));

    }

    for (int i = 0; i < 2000; i++) {
        float t = static_cast<float>(i)/1999.0f;
        glm::vec4 point;
        Bezier::findCurvePoint(point, controlPoints, t);

        pathPoints.push_back(glm::vec4(point));
    }


    return pathPoints;
}
