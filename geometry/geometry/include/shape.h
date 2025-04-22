#ifndef SHAPE_H
#define SHAPE_H

#include "geometry.h"

class Shape
{

private:
    int id = 0;

public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual void input() = 0;
    virtual void save(const std::string &filename) const = 0;
    virtual void draw() const = 0;
    virtual void translate(double dx, double dy, double dz = 0) = 0;
    virtual void scale(double sx, double sy, double sz = 0) = 0;
    virtual void rotate(const std::string &filename) = 0;

    int getId() const { return id; }
    void setId(int newId) { id = newId; }
};

#endif
