#include "geometry.h"

Bezier3D::Bezier3D() : controlPoints(), n(0) {}

Bezier3D::Bezier3D(std::vector<Point3D> controlPoints, int n) : controlPoints(controlPoints), n(n) {}

void Bezier3D::input()
{
    std::cout << "Enter the number of control points in the Bezier curve";
    int number_of_points;
    std::cin >> number_of_points;
    std::vector<Point3D> vectorpoints;
    for (int i = 0; i < number_of_points; i++)
    {
        std::cout << "Enter the coordinates of control point " << i + 1 << " (x,y,z) :";
        double x, y, z;
        std::cin >> x >> y >> z;
        Point3D point(x, y, z);
        vectorpoints.push_back(point);
    }
    n = number_of_points;
    controlPoints = vectorpoints;
}

void Bezier3D::draw() const
{
    system("gnuplot -e \"splot './geometry/data/bezier3d.txt' smooth csplines ; pause -1\"");
}

void Bezier3D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (int i = 0; i < n; i++)
        {
            file << controlPoints[i].getX() << " " << controlPoints[i].getY() << " " << controlPoints[i].getZ() << "\n";
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Bezier3D::translate(double dx, double dy, double dz)
{
    for (auto &point : controlPoints)
    {
        point.translate(dx, dy, dz);
    }
}

void Bezier3D::scale(double sx, double sy, double sz)
{
    for (auto &point : controlPoints)
    {
        point.translate(sx, sy, sz);
    }
}

void Bezier3D::rotate(const std::string &filename){

}