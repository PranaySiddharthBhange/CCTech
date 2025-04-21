#include "geometry.h"

Sphere::Sphere() : center(0, 0, 0), radius(0) {}

Sphere::Sphere(Point3D center, double radius) : center(center), radius(radius) {}

void Sphere::input()
{
    std::cout << "Enter the coordinates of the center of the sphere (x, y, z): ";
    double x, y, z;
    std::cin >> x >> y >> z;

    std::cout << "Enter the radius of the sphere: ";
    double r;
    std::cin >> r;

    center = Point3D(x, y, z);
    radius = r;
}

void Sphere::draw() const
{
    std::string command = "gnuplot -e \"splot './geometry/data/sphere.txt' with lines; pause -1\"";
    system(command.c_str());
}

void Sphere::save(const std::string &filename) const
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    const int u_samples = 30;
    const int v_samples = 20;
    const double pi = M_PI;

    // Loop through azimuthal angle (u) and polar angle (v)
    for (int i = 0; i <= u_samples; ++i)
    {
        double u = -pi + 2 * pi * i / u_samples; // Azimuthal angle
        for (int j = 0; j <= v_samples; ++j)
        {
            double v = -pi / 2 + pi * j / v_samples; // Polar angle

            // Parametric equations for the sphere
            double x = center.getX() + radius * cos(u) * cos(v);
            double y = center.getY() + radius * sin(u) * cos(v);
            double z = center.getZ() + radius * sin(v);

            // Write the point to the file with a precision of 6 decimal places
            file << x << " " << y << " " << z << "\n";
        }
        file << "\n"; // Separate rows
    }

    file.close();
}

void Sphere::translate(double dx, double dy, double dz)
{
    center.translate(dx, dy, dz);
}

void Sphere::scale(double factor, double sy, double sz)
{
    radius *= factor;
}

void Sphere::rotate(const std::string &filename)
{
    std::cout << "Rotation is not necessary for spheres.\n";
}
