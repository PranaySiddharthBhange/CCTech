#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> // Base class for creating main application windows in Qt

// Forward declarations of Qt classes to reduce compilation dependencies
class QComboBox;       // Dropdown menu for selecting shapes
class QListWidget;     // List widget for displaying shapes
class QDoubleSpinBox;  // Spin box for entering double-precision values
class GLScene;         // Custom class for rendering 3D shapes

// MainWindow class definition
class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro enabling Qt's signal and slot mechanism

public:
    // Constructor: Initializes the main window with an optional parent widget
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    // Slot: Adds a new shape to the scene
    void onAddShape();

    // Slot: Handles the selection of a shape from the list
    void onShapeSelected(int row);

    // Slot: Updates transformations (e.g., rotation, translation, scaling) of the selected shape
    void onTransformChanged();

    // Slot: Deletes the currently selected shape
    void onDeleteShape();

private:
    // Pointer to the GLScene object responsible for 3D rendering
    GLScene* glScene;

    // UI components for interacting with shapes
    QComboBox* shapeComboBox; // Dropdown menu for selecting shape types
    QListWidget* shapeListWidget; // List of shapes in the scene

    // Spin boxes for controlling rotation (X, Y, Z axes)
    QDoubleSpinBox* rotationXSpin, * rotationYSpin, * rotationZSpin;

    // Spin boxes for controlling translation (X, Y, Z axes)
    QDoubleSpinBox* translateXSpin, * translateYSpin, * translateZSpin;

    // Spin box for controlling uniform scaling
    QDoubleSpinBox* scaleSpin;
};

#endif // MAINWINDOW_H