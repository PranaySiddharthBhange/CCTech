#ifndef BEZIER3D_H
#define BEZIER3D_H

class Bezier3D : public Shape
{
private:
    std::vector<Point3D> controlPoints;
    int n;

public:
    Bezier3D();
    Bezier3D(std::vector<Point3D> controlPoints, int n);
    
    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double dx, double dy, double dz = 0) override;
    void rotate(const std::string &filename)override;

};
#endif