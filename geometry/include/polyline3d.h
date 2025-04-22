#ifndef POLYLINE3D_H
#define POLYLINE3D_H

class Polyline3D : public Shape
{
private:
    std::vector<Point3D> points;
    int n;

public:
    Polyline3D();
    Polyline3D(const std::vector<Point3D> &points, int n);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double dx, double dy, double dz = 0) override;
    void rotate(const std::string &filename)override;

};

#endif 