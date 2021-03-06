#pragma once

#include <vector>

#include "GraphicalObject.h"
#include "Vector2d.h"

namespace ample::graphics
{
using pixel_t = double;
using radians_t = double;

class GraphicalObject2d : public GraphicalObject
{
public:
    GraphicalObject2d(const std::vector<Vector2d<pixel_t>> &);

    virtual pixel_t getX() const = 0;
    virtual pixel_t getY() const = 0;
    virtual pixel_t getZ() const = 0;

    virtual radians_t getAngleX() const = 0;
    virtual radians_t getAngleY() const = 0;
    virtual radians_t getAngleZ() const = 0;

    virtual double getScaleX() const = 0;
    virtual double getScaleY() const = 0;
    virtual double getScaleZ() const = 0;

    void draw() override;

protected:
    void drawSelf() override;
    std::vector<Vector2d<pixel_t>> _graphicalShape;
};
} // namespace graphics
