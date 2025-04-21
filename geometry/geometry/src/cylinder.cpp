#include "geometry.h"

Cylinder::Cylinder() : center(0, 0, 0), radius(0), height(0) {}

Cylinder::Cylinder(Point3D center, double radius, double height) : center(center), radius(radius), height(height) {}

void Cylinder::input()
{
    std::cout << "Enter the coordinates of the center of the cylinder (x,y,z) :";
    double x, y, z;
    std::cin >> x >> y >> z;
    std::cout << "Enter the radius of the cylinder";
    double r;
    std::cin >> r;
    std::cout << "Enter the height of the cylinder";
    double h;
    std::cin >> h;

    center = Point3D(x, y, z);
    radius = r;
    height = h;
}

void Cylinder::draw() const
{
    system("gnuplot -e \"splot './geometry/data/cylinder.txt' with lines; pause -1\"");
}

void Cylinder::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (int i = 0; i <= 360; i++)
        {
            double x = center.getX() + radius * cos(i * M_PI / 180);
            double y = center.getY() + radius * sin(i * M_PI / 180);
            file << x << " " << y << " " << center.getZ() << std::endl;
        }
        file << std::endl;
        for (int i = 0; i <= 360; i++)
        {
            double x = center.getX() + radius * cos(i * M_PI / 180);
            double y = center.getY() + radius * sin(i * M_PI / 180);
            file << x << " " << y << " " << center.getZ() + height << std::endl;
        }
        file << std::endl;
        for (int i = 0; i <= 360; i++)
        {
            double x = center.getX() + radius * cos(i * M_PI / 180);
            double y = center.getY() + radius * sin(i * M_PI / 180);
            file << x << " " << y << " " << center.getZ() << std::endl;
            file << x << " " << y << " " << center.getZ() + height << std::endl;
            file << std::endl;
        }

        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Cylinder::translate(double dx, double dy, double dz)
{
    center.translate(dx, dy, dz);
}

void Cylinder::scale(double r, double h, double dz)
{
    radius *= r;
    height *= h;
}

void Cylinder::rotate(const std::string &filename)
{
    char axis;
    double angle;

    // Ask the user for the axis and the angle of rotation
    std::cout << "Enter the axis to rotate the cylinder (x, y, or z): ";
    std::cin >> axis;
    std::cout << "Enter the angle to rotate the cylinder (in degrees): ";
    std::cin >> angle;

    // Convert the angle from degrees to radians
    double theta = angle * M_PI / 180.0;

    // Open the file to save the rotated points
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Get the current center coordinates
    double x_center = center.getX();
    double y_center = center.getY();
    double z_center = center.getZ();

    // Rotate the center based on chosen axis
    if (axis == 'x' || axis == 'X')
    {
        // Rotation about X-axis:
        //   x' = x, y' = y*cos(theta) - z*sin(theta), z' = y*sin(theta) + z*cos(theta)
        double newY = y_center * cos(theta) - z_center * sin(theta);
        double newZ = y_center * sin(theta) + z_center * cos(theta);
        center.setY(newY);
        center.setZ(newZ);
    }
    else if (axis == 'y' || axis == 'Y')
    {
        // Rotation about Y-axis:
        //   x' = x*cos(theta) + z*sin(theta), y' = y, z' = -x*sin(theta) + z*cos(theta)
        double newX = x_center * cos(theta) + z_center * sin(theta);
        double newZ = -x_center * sin(theta) + z_center * cos(theta);
        center.setX(newX);
        center.setZ(newZ);
    }
    else if (axis == 'z' || axis == 'Z')
    {
        // Rotation about Z-axis:
        //   x' = x*cos(theta) - y*sin(theta), y' = x*sin(theta) + y*cos(theta), z' = z
        double newX = x_center * cos(theta) - y_center * sin(theta);
        double newY = x_center * sin(theta) + y_center * cos(theta);
        center.setX(newX);
        center.setY(newY);
    }
    else
    {
        std::cerr << "Invalid axis selection!" << std::endl;
        return;
    }

    // Define lambda functions for rotating a point depending on the axis
    auto rotatePoint = [theta, axis](double x, double y, double z, double &rx, double &ry, double &rz)
    {
        if (axis == 'x' || axis == 'X')
        {
            // Rotation about X-axis
            rx = x;
            ry = y * cos(theta) - z * sin(theta);
            rz = y * sin(theta) + z * cos(theta);
        }
        else if (axis == 'y' || axis == 'Y')
        {
            // Rotation about Y-axis
            rx = x * cos(theta) + z * sin(theta);
            ry = y;
            rz = -x * sin(theta) + z * cos(theta);
        }
        else if (axis == 'z' || axis == 'Z')
        {
            // Rotation about Z-axis
            rx = x * cos(theta) - y * sin(theta);
            ry = x * sin(theta) + y * cos(theta);
            rz = z;
        }
    };

    // Write the bottom circle points (rotated)
    for (int i = 0; i <= 360; i++)
    {
        // Calculate the original (x, y) coordinates on the bottom circle
        double x = x_center + radius * cos(i * M_PI / 180.0);
        double y = y_center + radius * sin(i * M_PI / 180.0);
        double z = z_center;

        double rx, ry, rz;
        rotatePoint(x, y, z, rx, ry, rz);
        file << rx << " " << ry << " " << rz << std::endl;
    }

    file << std::endl; // Separator between bottom and top circle

    // Write the top circle points (rotated)
    for (int i = 0; i <= 360; i++)
    {
        // Calculate the original (x, y) coordinates on the top circle
        double x = x_center + radius * cos(i * M_PI / 180.0);
        double y = y_center + radius * sin(i * M_PI / 180.0);
        double z = z_center + height;

        double rx, ry, rz;
        rotatePoint(x, y, z, rx, ry, rz);
        file << rx << " " << ry << " " << rz << std::endl;
    }

    file << std::endl; // Separator between top circle and side points

    // Write the side points (rotated)
    for (int i = 0; i <= 360; i++)
    {
        // Calculate the original (x, y) coordinates on the side of the cylinder
        double x = x_center + radius * cos(i * M_PI / 180.0);
        double y = y_center + radius * sin(i * M_PI / 180.0);
        double z_bottom = z_center;
        double z_top = z_center + height;

        double rx_bottom, ry_bottom, rz_bottom;
        double rx_top, ry_top, rz_top;

        // Rotate bottom point
        rotatePoint(x, y, z_bottom, rx_bottom, ry_bottom, rz_bottom);
        // Rotate top point
        rotatePoint(x, y, z_top, rx_top, ry_top, rz_top);

        file << rx_bottom << " " << ry_bottom << " " << rz_bottom << std::endl;
        file << rx_top << " " << ry_top << " " << rz_top << std::endl;
        file << std::endl; // Separator between side segments
    }

    file.close();
}
