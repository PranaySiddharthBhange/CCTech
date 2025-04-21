#include "geometry.h"

using namespace std;

void sketcherMenu()
{
    cout << "\n=============================== Sketcher Menu ===============================\n"<< endl;

    cout << "1. Add Shape          - Add a new shape to the sketch" << endl;
    cout << "2. Delete Shape       - Remove an existing shape from the sketch" << endl;
    cout << "3. Translate Shape    - Move a shape by a certain distance" << endl;
    cout << "4. Scale Shape        - Resize a shape by a scaling factor" << endl;
    cout << "5. Rotate Shape       - Rotate a shape by a angle along axis" << endl;
    cout << "6. Draw               - Draw all the shapes on the screen" << endl;
    cout << "7. Import STL         - Import STL file on the screen" << endl;
    cout << "8. Import OBJ         - Import OBJ file on the screen" << endl;
    cout << "9. Exit               - Exit the sketcher application" << endl;

    cout << "\n==============================================================================" << endl;

    cout << "Enter your choice: ";
}
