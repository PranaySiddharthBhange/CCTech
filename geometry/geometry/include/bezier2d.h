#ifndef BEZIER2D_H
#define BEZIER2D_H
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include "shape.h"
#include "main.h"
#include "point2d.h"
#include "point3d.h"
#include "line2d.h"
#include "line3d.h"
#include "rectangle.h"
#include "circle.h"
#include "cuboid.h"
#include "polyline2d.h"
#include "polyline3d.h"
#include "bezier2d.h"
#include "bezier3d.h"
#include "polygon.h"
#include "sphere.h"
#include "cylinder.h"
#include "sketcher.h"
#include "import.h"
class Bezier2D : public Shape
{
private:
    std::vector<Point2D> controlPoints;
    int n;

public:
    Bezier2D();
    Bezier2D(std::vector<Point2D> controlPoints, int n);

    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;

    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif