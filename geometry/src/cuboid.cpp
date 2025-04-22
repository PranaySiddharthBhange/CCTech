#include "geometry.h"
#define M_PI 3.14159265358979323846
Cuboid::Cuboid() : start(0, 0, 0), width(0), height(0), depth(0) {}

Cuboid ::Cuboid(Point3D &start, double width, double height, double depth) : start(start), width(width), height(height), depth(depth) {}

void Cuboid::input()
{
    std::cout << "Enter the starting coordinates of the cuboid (x1,y1,z1) :";
    double x, y, z;
    std::cin >> x >> y >> z;

    std::cout << "Enter the dimensions of the cuboid (width,height,depth) :";
    double w, h, d;
    std::cin >> w >> h >> d;

    start = Point3D(x, y, z);
    width = w;
    height = h;
    depth = d;
}

void Cuboid::draw() const
{

    system("gnuplot -e \"splot './geometry/data/cuboid.txt' with lines; pause -1\"");
}

void Cuboid::save(const std::string &filename) const
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        // Define the 8 vertices of the cuboid
        double vertices[8][3] = {
            {start.getX(), start.getY(), start.getZ()},                          // 0
            {start.getX() + width, start.getY(), start.getZ()},                  // 1
            {start.getX() + width, start.getY() + height, start.getZ()},         // 2
            {start.getX(), start.getY() + height, start.getZ()},                 // 3
            {start.getX(), start.getY(), start.getZ() + depth},                  // 4
            {start.getX() + width, start.getY(), start.getZ() + depth},          // 5
            {start.getX() + width, start.getY() + height, start.getZ() + depth}, // 6
            {start.getX(), start.getY() + height, start.getZ() + depth}          // 7
        };

        // Define the faces (each face is a set of 4 vertices)
        int faces[6][4] = {
            {0, 1, 2, 3}, // Bottom face
            {4, 5, 6, 7}, // Top face
            {0, 1, 5, 4}, // Front face
            {1, 2, 6, 5}, // Right face
            {2, 3, 7, 6}, // Back face
            {3, 0, 4, 7}  // Left face
        };

        // Write the faces to the file
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int vertexIndex = faces[i][j];
                file << vertices[vertexIndex][0] << " "
                     << vertices[vertexIndex][1] << " "
                     << vertices[vertexIndex][2] << "\n";
            }
            file << "\n"; // Separate faces with a blank line
        }

        file.close();
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Cuboid::translate(double dx, double dy, double dz)
{
    start.translate(dx, dy, dz);
}

void Cuboid::scale(double w, double h, double d)
{
    width *= w;
    height *= h;
    depth *= d;
}

void Cuboid::rotate(const std::string &filename)
{
    char axis;
    double angle;

    std::cout << "Enter the axis to rotate the cuboid (x, y, or z): ";
    std::cin >> axis;
    std::cout << "Enter the angle to rotate the cuboid (in degrees): ";
    std::cin >> angle;

    double theta = angle * M_PI / 180.0;

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Define the 8 vertices of the cuboid
    double vertices[8][3] = {
        {start.getX(), start.getY(), start.getZ()},                          // 0
        {start.getX() + width, start.getY(), start.getZ()},                  // 1
        {start.getX() + width, start.getY() + height, start.getZ()},         // 2
        {start.getX(), start.getY() + height, start.getZ()},                 // 3
        {start.getX(), start.getY(), start.getZ() + depth},                  // 4
        {start.getX() + width, start.getY(), start.getZ() + depth},          // 5
        {start.getX() + width, start.getY() + height, start.getZ() + depth}, // 6
        {start.getX(), start.getY() + height, start.getZ() + depth}          // 7
    };

    // Define the faces (each face is a set of 4 vertices)
    int faces[6][4] = {
        {0, 1, 2, 3}, // Bottom face
        {4, 5, 6, 7}, // Top face
        {0, 1, 5, 4}, // Front face
        {1, 2, 6, 5}, // Right face
        {2, 3, 7, 6}, // Back face
        {3, 0, 4, 7}  // Left face
    };

    // Define lambda function for rotating a point based on the selected axis
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

    // Array to store rotated vertices
    double rotatedVertices[8][3];

    // Apply rotation to each vertex
    for (int i = 0; i < 8; i++)
    {
        rotatePoint(vertices[i][0], vertices[i][1], vertices[i][2],
                    rotatedVertices[i][0], rotatedVertices[i][1], rotatedVertices[i][2]);
    }

    // Write the rotated faces to the file
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int vertexIndex = faces[i][j];
            file << rotatedVertices[vertexIndex][0] << " "
                 << rotatedVertices[vertexIndex][1] << " "
                 << rotatedVertices[vertexIndex][2] << "\n";
        }
        file << "\n"; // Separate faces with a blank line
    }

    file.close();
}
