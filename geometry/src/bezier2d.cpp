#include "geometry.h"

Bezier2D::Bezier2D() : controlPoints(), n(0) {}

Bezier2D::Bezier2D(std::vector<Point2D> controlPoints, int n) : controlPoints(controlPoints), n(n) {}

void Bezier2D::input()
{
    std::cout << "Enter the number of control points in the Bezier curve";
    int number_of_points;
    std::cin >> number_of_points;
    std::vector<Point2D> vectorpoints;
    for (int i = 0; i < number_of_points; i++)
    {
        std::cout << "Enter the coordinates of control point " << i + 1 << " (x,y) :";
        double x, y;
        std::cin >> x >> y;
        Point2D point(x, y);
        vectorpoints.push_back(point);
    }

    n = number_of_points;
    controlPoints = vectorpoints;
}

void Bezier2D::draw() const
{
    system("gnuplot -e \"plot './geometry/data/bezier2d.txt' smooth bezier; pause -1\"");
}

void Bezier2D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (int i = 0; i < n; i++)
        {
            file << controlPoints[i].getX() << " " << controlPoints[i].getY() << "\n";
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Bezier2D::translate(double dx, double dy, double dz)
{
    for (auto &point : controlPoints)
    {
        point.translate(dx, dy);
    }
}

void Bezier2D::scale(double sx, double sy, double sz)
{
    for (auto &point : controlPoints)
    {
        point.scale(sx, sy);
    }
}

void Bezier2D::rotate(const std::string &filename){

}