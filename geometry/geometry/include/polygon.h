#ifndef POLYGON_H
#define POLYGON_H

class Polygon : public Shape
{

private:
    std::vector<Point2D> points;
    int n;

public:
    Polygon();
    Polygon(std::vector<Point2D> points, int n);
    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double dx, double dy, double dz = 0) override;
    void rotate(const std::string &filename) override;
};
#endif