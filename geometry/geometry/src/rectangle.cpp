#include "geometry.h"

Rectangle::Rectangle() : start(0, 0), width(0), height(0) {}

Rectangle::Rectangle(Point2D start, double width, double height) : start(start), width(width), height(height) {}

void Rectangle::input()
{
    std::cout << "Enter the starting coordinates of the rectangle (x1,y1) :";
    double inputX, inputY;
    std::cin >> inputX >> inputY;

    std::cout << "Enter the dimensions of the rectangle (width,height) :";
    double inputW, inputH;
    std::cin >> inputW >> inputH;

    start = Point2D(inputX, inputY);

    width = inputW;
    height = inputH;
}

void Rectangle::draw() const
{
    system("gnuplot -e \"plot './geometry/data/rectangle.txt' with lines; pause -1\"");
}

void Rectangle::save(const std::string &filename) const
{

    std::ofstream file(filename);

    if (file)
    {
        // 4 vertices of the rectangle
        double vertices[4][2] = {
            {start.getX(), start.getY()},                  // 0
            {start.getX() + width, start.getY()},          // 1
            {start.getX() + width, start.getY() + height}, // 2
            {start.getX(), start.getY() + height}          // 3
        };

        for (int i = 0; i < 4; i++)
        {
            file << vertices[i][0] << " " << vertices[i][1] << "\n";
        }
        file << vertices[0][0] << " " << vertices[0][1] << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Rectangle::translate(double dx, double dy, double dz)
{
    start.translate(dx, dy);
}

void Rectangle::scale(double w, double h, double dz)
{
    width *= w;
    height *= h;
}

void Rectangle::rotate(const std::string &filename)
{
}