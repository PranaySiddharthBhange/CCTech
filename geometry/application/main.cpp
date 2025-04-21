#include "geometry.h"
#include <filesystem>
#include <iostream>

int main()
{

    Sketcher sketcher;
    int choice;

    do
    {
        sketcherMenu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            sketcher.addShape();
            break;
        case 2:
            sketcher.deleteShape();
            break;
        case 3:
            sketcher.translateShape();
            break;
        case 4:
            sketcher.scaleShape();
            break;
        case 5:
            sketcher.rotateShape();
            break;
        case 6:
            sketcher.draw();
            break;
        case 7:
            sketcher.importSTL();
            break;

        case 8:
            sketcher.importOBJ();
            break;
        case 9:
            std::cout << "Exiting Sketcher...\n";
            break;
        default:
            std::cout << "Invalid choice! Please enter a number between 1-6." << std::endl;
            break;
        }
    } while (choice != 9);

    return 0;
}
