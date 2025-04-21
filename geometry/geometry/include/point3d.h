#ifndef POINT3D_H
#define POINT3D_H

class Point3D : public Point2D
{
private:
    double z;

public:
    Point3D();
    Point3D(double x, double y, double z);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;

    double getZ() const;
    void setZ(double z);
};

#endif