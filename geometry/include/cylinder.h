#ifndef CYLINDDER_H
#define CYLINDDER_H

class Cylinder : public Shape
{
private:
    Point3D center;
    double radius;
    double height;

public:
    Cylinder();
    Cylinder(Point3D center, double radius, double height);

    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif