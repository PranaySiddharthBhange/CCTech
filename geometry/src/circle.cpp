#include "geometry.h"

Circle::Circle() : center(0, 0), radius(0) {}

Circle::Circle(Point2D &center, double radius) : center(center), radius(radius) {}

void Circle::input()
{
    std::cout << "Enter the coordinates of the center of the circle (x,y) :";

    double x, y;
    std::cin >> x >> y;

    std::cout << "Enter the radius of the circle :";
    double r;
    std::cin >> r;

    center = Point2D(x, y);
    radius = r;
}

void Circle::draw() const
{

    system("gnuplot -e \"plot './geometry/data/circle.txt' with circle; pause -1\"");
}

void Circle::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file)
    {
        file << center.getX() << " " << center.getY() << " " << radius << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Circle::translate(double dx, double dy, double dz)
{
    center.translate(dx, dy);
}

void Circle::scale(double sx, double sy, double sz)
{
    radius *= sx;
}

void Circle::rotate(const std::string &filename)
{
}