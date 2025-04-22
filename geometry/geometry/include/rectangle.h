#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle : public Shape
{
private:
    Point2D start; // top-left point
    double width, height;

public:
    Rectangle();
    Rectangle(Point2D start, double width, double height);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif 
