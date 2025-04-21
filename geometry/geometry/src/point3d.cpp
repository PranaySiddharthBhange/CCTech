#include "geometry.h"

Point3D::Point3D() : Point2D(), z(0) {}

Point3D::Point3D(double x, double y, double z) : Point2D(x, y), z(z) {}

double Point3D::getZ() const
{
    return z;
}

void Point3D::setZ(double z)
{
    this->z = z;
}

void Point3D::input()
{
    std::cout << "Enter the coordinates of the point (x,y,z):";
    double inputX, inputY, inputZ;
    std::cin >> inputX >> inputY >> inputZ;

    setX(inputX);
    setY(inputY);
    setZ(inputZ);
}

void Point3D::draw() const
{
    system("gnuplot -e \"splot './geometry/data/point3d.txt' with points; pause -1\"");
}

void Point3D::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file)
    {
        file << getX() << " " << getY() << " " << getZ() << "\n";
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Point3D::translate(double dx, double dy, double dz)
{
    Point2D::translate(dx, dy);
    z += dz;
}

void Point3D::scale(double dx, double dy, double dz)
{
    Point2D::scale(dx, dy);
    z *= dz;
}

void Point3D::rotate(const std::string &filename)
{
    double angle;
    std::cout << "Enter the angle of rotation in degrees: ";
    std::cin >> angle;

    double rad = angle * M_PI / 180.0;

    char axis;
    std::cout << "Enter the axis of rotation (X, Y, or Z): ";
    std::cin >> axis;

    if (axis == 'X' || axis == 'x') {

        double newY = getY() * cos(rad) - getZ() * sin(rad);
        double newZ = getY() * sin(rad) + getZ() * cos(rad);
        setY(newY);
        setZ(newZ);
    } 
    else if (axis == 'Y' || axis == 'y') {

        double newX = getX() * cos(rad) + getZ() * sin(rad);
        double newZ = -getX() * sin(rad) + getZ() * cos(rad);
        setX(newX);
        setZ(newZ);
    }
    else if (axis == 'Z' || axis == 'z') {

        double newX = getX() * cos(rad) - getY() * sin(rad);
        double newY = getX() * sin(rad) + getY() * cos(rad);
        setX(newX);
        setY(newY);
    } 
    else {
        std::cerr << "Invalid axis. Please enter X, Y, or Z." << std::endl;
        return; 
    }

}