#ifndef LINE2D_H
#define LINE2D_H

class Line2D : public Shape
{
private:
    Point2D start, end;

public:
    Line2D();
    Line2D(Point2D start, Point2D end);

    void input() override;
    void save(const std::string &filename) const override;
    void draw() const override;
    void translate(double dx, double dy, double dz = 0) override;
    void scale(double sx, double sy, double sz = 0) override;
    void rotate(const std::string &filename) override;
};

#endif
