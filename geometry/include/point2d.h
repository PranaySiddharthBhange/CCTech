#ifndef POINT2D_H
#define POINT2D_H

class Point2D : public Shape
{
private:
    double x, y;

public:
    Point2D();
    Point2D(double x, double y);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;

    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
};

#endif
