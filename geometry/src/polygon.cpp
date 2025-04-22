#include "geometry.h"

Polygon::Polygon() : points(), n(0) {}

Polygon ::Polygon(std::vector<Point2D> points, int n) : points(points), n(n) {}

void Polygon ::input()
{
    std::cout << "Enter the number of points in the polygon (should be greater than 3 for triangle type 3) :";
    int number_of_points;
    std::cin >> number_of_points;
    std::vector<Point2D> vectorpoints;
    for (int i = 0; i < number_of_points; i++)
    {
        std::cout << "Enter the coordinates of point " << i + 1 << " (x,y) :";
        double x, y;
        std::cin >> x >> y;
        Point2D point(x, y);
        vectorpoints.push_back(point);
    }
    n = number_of_points;
    points = vectorpoints;
}

void Polygon::draw() const
{
    system("gnuplot -e \"plot './geometry/data/polygon.txt' with lines; pause -1\"");
}

void Polygon ::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (int i = 0; i < n; i++)
        {
            file << points[i].getX() << " " << points[i].getY() << std::endl;
        }
        file << points[0].getX() << " " << points[0].getY() << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Polygon ::translate(double dx, double dy, double dz)
{
    for (auto &point : points)
    {
        point.translate(dx, dy);
    }
}

void Polygon ::scale(double sx, double sy, double sz)
{
    for (auto &point : points)
    {
        point.translate(sx, sy);
    }
}

void Polygon::rotate(const std::string &filename){

}