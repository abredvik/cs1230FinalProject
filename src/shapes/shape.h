#pragma once

#include <vector>

class Shape
{
public:
    virtual void updateParams(int param1, int param2) = 0;

    virtual const std::vector<float>& generateShape() const = 0;

    virtual const int getNumVertices() const = 0;

protected:
    Shape();

};
