#ifndef LINE3D_H
#define LINE3D_H

class Line3D : public Shape
{
private:
    Point3D start, end;

public:
    Line3D();
    Line3D(Point3D start, Point3D end);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif
