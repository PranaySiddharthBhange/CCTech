#ifndef BEZIER2D_H
#define BEZIER2D_H
class Bezier2D : public Shape
{
private:
    std::vector<Point2D> controlPoints;
    int n;

public:
    Bezier2D();
    Bezier2D(std::vector<Point2D> controlPoints, int n);

    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;

    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif