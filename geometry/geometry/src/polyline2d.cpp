#include "geometry.h"

Polyline2D::Polyline2D() : points(), n(0) {}

Polyline2D::Polyline2D(const std::vector<Point2D> &points, int n) : points(points), n(n) {}

void Polyline2D::input()
{

    std::cout << "Enter the number of points in the 2D polyline :";
    int number_of_points;

    std::cin >> number_of_points;
    std::vector<Point2D> vector_points;
    for (int i = 0; i < number_of_points; i++)
    {
        std::cout << "Enter the coordinates of point " << i + 1 << " (x,y) :";
        double x, y;
        std::cin >> x >> y;
        Point2D point(x, y);
        vector_points.push_back(point);
    }
    n = number_of_points;
    points = vector_points;
}

void Polyline2D::draw() const
{
    system("gnuplot -e \"plot './geometry/data/polyline2d.txt' with lines; pause -1\"");
}

void Polyline2D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (auto &point : points)
        {
            file << point.getX() << " " << point.getY() << "\n";
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Polyline2D::translate(double dx, double dy, double dz)
{
    for (auto &point : points)
    {
        point.translate(dx, dy);
    }
}

void Polyline2D::scale(double sx, double sy, double sz)
{
    for (auto &point : points)
    {
        point.scale(sx, sy);
    }
}

void Polyline2D::rotate(const std::string &filename){

}