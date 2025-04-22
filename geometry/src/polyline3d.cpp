#include "geometry.h"

Polyline3D::Polyline3D() : points(), n(0) {}

Polyline3D::Polyline3D(const std::vector<Point3D> &points, int n) : points(points), n(n) {}

void Polyline3D ::input()
{
    std::cout << "Enter the number of points in the polyline :";
    int number_of_points;
    std::cin >> number_of_points;
    std::vector<Point3D> vectorpoints;
    for (int i = 0; i < number_of_points; i++)
    {
        std::cout << "Enter the coordinates of point " << i + 1 << " (x,y,z) :";
        double x, y, z;
        std::cin >> x >> y >> z;
        Point3D point(x, y, z);
        vectorpoints.push_back(point);
    }
    n = number_of_points;
    points = vectorpoints;
}

void Polyline3D::draw() const
{
    system("gnuplot -e \"splot './geometry/data/polyline3d.txt' with lines; pause -1\"");
}

void Polyline3D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (auto &point : points)
        {
            file << point.getX() << " " << point.getY() << " " << point.getZ() << "\n";
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Polyline3D ::translate(double dx, double dy, double dz)
{
    for (auto &point : points)
    {
        point.translate(dx, dy, dz);
    }
}

void Polyline3D ::scale(double dx, double dy, double dz)
{
    for (auto &point : points)
    {
        point.scale(dx, dy, dz);
    }
}

void Polyline3D::rotate(const std::string &filename){

}