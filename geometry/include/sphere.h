#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Shape
{

private:
    Point3D center;
    double radius;

public:
    Sphere();
    Sphere(Point3D center, double radius);
    
    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double dx, double dy, double dz = 0) override;
    void rotate(const std::string &filename)override;

};
#endif 