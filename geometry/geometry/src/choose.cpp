#include "geometry.h"

using namespace std;

void chooseShape()
{

    switch (displayMenu())
    {
    case 1:
    {
        Point2D point;
        point.input();
        point.save("./geometry/data/point2d.txt");
        point.draw();
        break;
    }
    case 2:
    {
        Point3D point;
        point.input();
        point.save("./geometry/data/point3d.txt");
        point.draw();
        break;
    }
    case 3:
    {
        Line2D line;
        line.input();
        line.save("./geometry/data/line2d.txt");
        line.draw();
        break;
    }
    case 4:
    {
        Line3D line;
        line.input();
        line.save("./geometry/data/line3d.txt");
        line.draw();
        break;
    }
    case 5:
    {
        Rectangle rectangle;
        rectangle.input();
        rectangle.save("./geometry/data/rectangle.txt");
        rectangle.draw();
        break;
    }
    case 6:
    {
        Circle circle;
        circle.input();
        circle.save("./geometry/data/circle.txt");
        circle.draw();
        break;
    }
    case 7:
    {
        Cuboid cuboid;
        cuboid.input();
        cuboid.save("./geometry/data/cuboid.txt");
        cuboid.rotate("./geometry/data/cuboid.txt");
        cuboid.draw();
        break;
    }
    case 8:
    {
        cout << "Enter the starting coordinate of the cube (x, y, z): ";
        double x, y, z;
        cin >> x >> y >> z;

        cout << "Enter the length of the Cube: ";
        double length;
        cin >> length;

        Point3D start(x, y, z);

        Cuboid cube(start, length, length, length);

        cube.save("./geometry/data/cuboid.txt");
        cube.draw();
    }
    case 9:
    {
        Polyline2D polyline2d;
        polyline2d.input();
        polyline2d.save("./geometry/data/polyline2d.txt");
        polyline2d.draw();
        break;
    }
    case 10:
    {
        Polyline3D polyline3d;
        polyline3d.input();
        polyline3d.save("./geometry/data/polyline3d.txt");
        polyline3d.draw();
        break;
    }
    case 11:
    {
        Bezier2D bezier2d;
        bezier2d.input();
        bezier2d.save("./geometry/data/bezier2d.txt");
        bezier2d.draw();
        break;
    }
    case 12:
    {
        Bezier3D bezier3d;
        bezier3d.input();
        bezier3d.save("./geometry/data/bezier3d.txt");
        bezier3d.draw();
        break;
    }
    case 13:
    {
        Polygon polygon;
        polygon.input();
        polygon.save("./geometry/data/polygon.txt");
        polygon.draw();
        break;
    }
    case 14:
    {
        Polygon triangle;
        triangle.input();
        triangle.save("./geometry/data/polygon.txt");
        triangle.draw();
        break;
    }
    case 15:
    {
        Sphere sphere;
        sphere.input();
        sphere.save("./geometry/data/sphere.txt");
        sphere.draw();
        break;
    }
    case 16:
    {
        Cylinder cylinder;
        cylinder.input();
        cylinder.save("./geometry/data/cylinder.txt");
        cylinder.rotate("./geometry/data/cylinder.txt"); 
        cylinder.draw();
    }
    default:
    {
        cout << "Invalid choice" << endl;
        break;
    }
    }
}
