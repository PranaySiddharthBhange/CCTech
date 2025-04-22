#include "geometry.h"

Line3D::Line3D() : start(0, 0, 0), end(0, 0, 0) {}

Line3D::Line3D(Point3D start, Point3D end) : start(start), end(end) {}

void Line3D::input()
{
    std::cout << "Enter the starting and ending coordinates of the line (x1,y1,z1,x2,y2,z2) :";

    double inputX1, inputY1, inputZ1, inputX2, inputY2, inputZ2;
    std::cin >> inputX1 >> inputY1 >> inputZ1 >> inputX2 >> inputY2 >> inputZ2;

    start = Point3D(inputX1, inputY1, inputZ1);
    end = Point3D(inputX2, inputY2, inputZ2);
}

void Line3D::draw() const
{
    system("gnuplot -e \"splot './geometry/data/line3d.txt' with linespoints; pause -1\"");
}

void Line3D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file)
    {

        for (int i = 0; i <= 8; i++)
        {
            double t = i / 8.0;
            double xi = start.getX() + t * (end.getX() - start.getX());
            double yi = start.getY() + t * (end.getY() - start.getY());
            double zi = start.getZ() + t * (end.getZ() - start.getZ());
            file << xi << " " << yi << " " << zi << "\n";
        }
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Line3D::translate(double dx, double dy, double dz)
{
    start.translate(dx, dy, dz);
    end.translate(dx, dy, dz);
}

void Line3D::scale(double sx, double sy, double sz)
{
    start.scale(sx, sy, sz);
    end.scale(sx, sy, sz);
}

void Line3D::rotate(const std::string &filename)
{
    double angle;
    std::cout << "Enter the angle of rotation in degrees: ";
    std::cin >> angle;

    double rad = angle * M_PI / 180.0;

    char axis;
    std::cout << "Enter the axis of rotation (X, Y, or Z): ";
    std::cin >> axis;

    double startX = start.getX();
    double startY = start.getY();
    double startZ = start.getZ();
    double newStartX, newStartY, newStartZ;

    if (axis == 'X' || axis == 'x')
    {
        newStartX = startX;
        newStartY = startY * cos(rad) - startZ * sin(rad);
        newStartZ = startY * sin(rad) + startZ * cos(rad);
    }
    else if (axis == 'Y' || axis == 'y')
    {
        newStartX = startX * cos(rad) + startZ * sin(rad);
        newStartY = startY;
        newStartZ = -startX * sin(rad) + startZ * cos(rad);
    }
    else if (axis == 'Z' || axis == 'z')
    {
        newStartX = startX * cos(rad) - startY * sin(rad);
        newStartY = startX * sin(rad) + startY * cos(rad);
        newStartZ = startZ;
    }
    else
    {
        std::cerr << "Invalid axis. Please enter X, Y, or Z." << std::endl;
        return;
    }

    start.setX(newStartX);
    start.setY(newStartY);
    start.setZ(newStartZ);

    double endX = end.getX();
    double endY = end.getY();
    double endZ = end.getZ();
    double newEndX, newEndY, newEndZ;

    if (axis == 'X' || axis == 'x')
    {
        newEndX = endX;
        newEndY = endY * cos(rad) - endZ * sin(rad);
        newEndZ = endY * sin(rad) + endZ * cos(rad);
    }
    else if (axis == 'Y' || axis == 'y')
    {
        newEndX = endX * cos(rad) + endZ * sin(rad);
        newEndY = endY;
        newEndZ = -endX * sin(rad) + endZ * cos(rad);
    }
    else if (axis == 'Z' || axis == 'z')
    {
        newEndX = endX * cos(rad) - endY * sin(rad);
        newEndY = endX * sin(rad) + endY * cos(rad);
        newEndZ = endZ;
    }

    end.setX(newEndX);
    end.setY(newEndY);
    end.setZ(newEndZ);

    std::ofstream file(filename);
    if (file)
    {
        file << start.getX() << " " << start.getY() << " " << start.getZ() << "\n";
        file << end.getX() << " " << end.getY() << " " << end.getZ() << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}