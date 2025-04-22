#ifndef CIRCLE_H
#define CIRCLE_H

class Circle : public Shape
{
private:
    Point2D center;
    double radius;

public:
    Circle();
    Circle(Point2D &center, double radius);
    
    void save(const std::string &filename) const override;
    void input() override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};
#endif 