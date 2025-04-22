#ifndef CUBOID_H
#define CUBOID_H

class Cuboid : public Shape
{
private:
    Point3D start;
    double width, height, depth;

public:
    Cuboid();
    Cuboid(Point3D &start, double width, double height, double depth);

    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif
