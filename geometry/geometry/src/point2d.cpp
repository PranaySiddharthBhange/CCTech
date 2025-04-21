#include "geometry.h"

Point2D::Point2D() : x(0), y(0) {}

Point2D::Point2D(double x, double y) : x(x), y(y) {}

double Point2D::getX() const
{
    return x;
}

double Point2D::getY() const
{
    return y;
}

void Point2D::setX(double x)
{
    this->x = x;
}

void Point2D::setY(double y)
{
    this->y = y;
}

void Point2D::input()
{
    std::cout << "Enter the coordinates of the point (x,y): ";
    double inputX, inputY;
    std::cin >> inputX >> inputY;

    setX(inputX);
    setY(inputY);
}

void Point2D::draw() const
{
    system("gnuplot -e \"plot './geometry/data/point2d.txt' with points; pause -1\"");
}

void Point2D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file)
    {
        file << x << " " << y << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Point2D::translate(double dx, double dy, double dz)
{
    x += dx;
    y += dy;
}

void Point2D::scale(double dx, double dy, double dz)
{
    x *= dx;
    y *= dy;
}

void Point2D::rotate(const std::string &filename)
{

    double angle;
    std::cout << "Enter the angle of rotation in degrees: ";
    std::cin >> angle;

    double rad = angle * M_PI / 180.0;

    double newX = x * cos(rad) - y * sin(rad);
    double newY = x * sin(rad) + y * cos(rad);

    x = newX;
    y = newY;

    std::ofstream file(filename);
    if (file)
    {
        file << x << " " << y << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}
