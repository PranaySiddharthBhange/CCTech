#include "geometry.h"

Line2D::Line2D() : start(Point2D(0, 0)), end(Point2D(0, 0)) {}

Line2D::Line2D(Point2D start, Point2D end) : start(start), end(end) {}

void Line2D::input()
{
    std::cout << "Enter the starting and ending coordinates of the line (x1,y1,x2,y2) :";
    double inputX1, inputY1, inputX2, inputY2;
    std::cin >> inputX1 >> inputY1 >> inputX2 >> inputY2;

    start = Point2D(inputX1, inputY1);
    end = Point2D(inputX2, inputY2);
}

void Line2D::draw() const
{
    system("gnuplot -e \"plot './geometry/data/line2d.txt' with linespoints; pause -1\"");
}

void Line2D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file)
    {
        for (int i = 0; i <= 8; i++)
        {
            double t = i / 8.0;
            double xi = start.getX() + t * (end.getX() - start.getX());
            double yi = start.getY() + t * (end.getY() - start.getY());
            file << xi << " " << yi << "\n";
        }
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Line2D::translate(double dx, double dy, double dz)
{
    start.translate(dx, dy);
    end.translate(dx, dy);
}

void Line2D::scale(double sx, double sy, double sz)
{
    start.scale(sx, sy);
    end.scale(sx, sy);
}

void Line2D::rotate(const std::string &filename)
{
    double angle;
    std::cout << "Enter the angle of rotation in degrees: ";
    std::cin >> angle;

    double rad = angle * M_PI / 180.0;

    double startX = start.getX();
    double startY = start.getY();
    double newStartX = startX * cos(rad) - startY * sin(rad);
    double newStartY = startX * sin(rad) + startY * cos(rad);
    start.setX(newStartX);
    start.setY(newStartY);

    double endX = end.getX();
    double endY = end.getY();
    double newEndX = endX * cos(rad) - endY * sin(rad);
    double newEndY = endX * sin(rad) + endY * cos(rad);
    end.setX(newEndX);
    end.setY(newEndY);


    std::ofstream file(filename);
    if (file)
    {
        file << start.getX() << " " << start.getY() << "\n";
        file << end.getX() << " " << end.getY() << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}