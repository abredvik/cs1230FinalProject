#include "bezier.h"
#include "glm/fwd.hpp"
#include <iostream>

Bezier::Bezier()
{

}

//Bezier::drawCurvePoint(std::vector(glm::vec4), t):
//    if(points.length==1):
//    draw(points[0])
//    else:
//           newpoints=array(points.size-1)
//           for(i=0; i<newpoints.length; i++):
//    x = (1-t) * points[i].x + t * points[i+1].x
//                                    y = (1-t) * points[i].y + t * points[i+1].y
//                                  newpoints[i] = new point(x,y)
//    drawCurvePoint(newpoints, t)
