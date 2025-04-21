#include "geometry.h"
using namespace std;

int displayMenu()
{
    cout << "\n\nWhat do you want to plot? \n\n"<< endl;
    cout << "Choose from the following options: " << endl;
    cout << "------------------------------------" << endl;
    cout << "1. 2D Point" << endl;
    cout << "2. 3D Point" << endl;
    cout << "3. 2D Line" << endl;
    cout << "4. 3D Line" << endl;
    cout << "5. Rectangle" << endl;
    cout << "6. Circle" << endl;
    cout << "7. Cuboid" << endl;
    cout << "8. Cube" << endl;
    cout << "9. Polyline 2D" << endl;
    cout << "10. Polyline 3D" << endl;
    cout << "11. Bezier Curve 2D" << endl;
    cout << "12. Bezier Curve 3D" << endl;
    cout << "13. Polygon" << endl;
    cout << "14. Triangle" << endl;
    cout << "15. Sphere" << endl;
    cout << "16. Cylinder\n"<< endl;

    cout << "------------------------------------" << endl;
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;

    return choice;
}
