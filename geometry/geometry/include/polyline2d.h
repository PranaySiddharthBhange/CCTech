#ifndef POLYLINE2D_H
#define POLYLINE2D_H

class Polyline2D : public Shape
{
private:
    std::vector<Point2D> points;
    int n;

public:
    Polyline2D();
    Polyline2D(const std::vector<Point2D> &points, int n);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double dx, double dy, double dz = 0) override;
    void rotate(const std::string &filename) override;
};
#endif