#include "mainwindow.h"
#include "glscene.h"
#include <QDockWidget>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QVariant>
#include <QMessageBox>
// Constructor: Initializes the main window and its UI components
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Create the central OpenGL scene widget
    glScene = new GLScene(this);
    setCentralWidget(glScene);

    // Create a control panel for managing shapes and transformations
    QWidget* controlPanel = new QWidget(this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);

    // --- Shape Selection ---
     //Dropdown menu for selecting the type of shape to add
    shapeComboBox = new QComboBox(controlPanel);
    shapeComboBox->addItem("Cube", QVariant::fromValue(Cube));
    shapeComboBox->addItem("Sphere", QVariant::fromValue(Sphere));
    shapeComboBox->addItem("Cylinder", QVariant::fromValue(Cylinder));
    shapeComboBox->addItem("Cone", QVariant::fromValue(Cone));
    shapeComboBox->addItem("Custom", QVariant::fromValue(Custom)); // Add custom shape option

    controlLayout->addWidget(shapeComboBox);



    // controlLayout->addWidget(shapeComboBox);

    // Button to add a new shape to the scene
    QPushButton* addShapeButton = new QPushButton("Add Shape", controlPanel);
    controlLayout->addWidget(addShapeButton);
    connect(addShapeButton, &QPushButton::clicked, this, &MainWindow::onAddShape);

	
    // List widget to display the shapes added to the scene
    shapeListWidget = new QListWidget(controlPanel);
    controlLayout->addWidget(shapeListWidget);
    connect(shapeListWidget, &QListWidget::currentRowChanged, this, &MainWindow::onShapeSelected);


    // --- Transformation Controls ---
    // Group box for transformation controls (rotation, translation, scaling)
    QGroupBox* transformGroup = new QGroupBox("Transformations", controlPanel);
    QFormLayout* formLayout = new QFormLayout(transformGroup);

    // Spin boxes for rotation (X, Y, Z axes)
    rotationXSpin = new QDoubleSpinBox(transformGroup);
    rotationXSpin->setRange(-360, 360);
    formLayout->addRow("Rotation X:", rotationXSpin);
    connect(rotationXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    rotationYSpin = new QDoubleSpinBox(transformGroup);
    rotationYSpin->setRange(-360, 360);
    formLayout->addRow("Rotation Y:", rotationYSpin);
    connect(rotationYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    rotationZSpin = new QDoubleSpinBox(transformGroup);
    rotationZSpin->setRange(-360, 360);
    formLayout->addRow("Rotation Z:", rotationZSpin);
    connect(rotationZSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    // Spin boxes for translation (X, Y, Z axes)
    translateXSpin = new QDoubleSpinBox(transformGroup);
    translateXSpin->setRange(-100, 100);
    formLayout->addRow("Translate X:", translateXSpin);
    connect(translateXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    translateYSpin = new QDoubleSpinBox(transformGroup);
    translateYSpin->setRange(-100, 100);
    formLayout->addRow("Translate Y:", translateYSpin);
    connect(translateYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    translateZSpin = new QDoubleSpinBox(transformGroup);
    translateZSpin->setRange(-100, 100);
    translateZSpin->setValue(-5); // Default Z translation
    formLayout->addRow("Translate Z:", translateZSpin);
    connect(translateZSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    // Spin box for uniform scaling
    scaleSpin = new QDoubleSpinBox(transformGroup);
    scaleSpin->setRange(0.1, 10);
    scaleSpin->setSingleStep(0.1);
    formLayout->addRow("Scale:", scaleSpin);
    connect(scaleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    controlLayout->addWidget(transformGroup);

    // --- Delete Shape Button ---
    // Button to delete the currently selected shape
    QPushButton* deleteShapeButton = new QPushButton("Delete Shape", controlPanel);
    controlLayout->addWidget(deleteShapeButton);
    connect(deleteShapeButton, &QPushButton::clicked, this, &MainWindow::onDeleteShape);

    // Set up the control panel as a dockable widget
    controlPanel->setLayout(controlLayout);
    QDockWidget* dock = new QDockWidget("Controls", this);
    dock->setWidget(controlPanel);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::onAddShape() {
    ShapeType type = static_cast<ShapeType>(shapeComboBox->currentData().toInt());

    if (type == ShapeType::Custom) {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("Open 3D Model File"),
            "",
            tr("3D Model Files (*.stl *.obj *.txt);;All Files (*)")
        );

        if (filePath.isEmpty()) {
            return; // User cancelled the dialog
        }
		glScene->makeCurrent(); // Make the OpenGL context current

  //      glScene->setupCustomGeometry(filePath.toStdString()); // Load and set custom geometry

		//glScene->addShape(type); // Add the custom shape to the scene

        GLShape shape = glScene->setupCustomGeometry(filePath.toStdString());
        glScene->shapes.push_back(shape);
        glScene->selectedShapeIndex = glScene->shapes.size() - 1;

		glScene->doneCurrent(); // Release the OpenGL context

        shapeListWidget->addItem(QFileInfo(filePath).fileName()); // Add file name to the shape list
        
    }
    else {
        glScene->addShape(type); // Add predefined shape
        shapeListWidget->addItem(shapeComboBox->currentText()); // Add shape name to the list
    }

    // Select the newly added shape in the list
    shapeListWidget->setCurrentRow(static_cast<int>(glScene->shapes.size()) - 1);

    glScene->update(); // Trigger a redraw of the OpenGL scene
}

// Slot: Updates the transformation controls when a shape is selected
void MainWindow::onShapeSelected(int row) {
    glScene->selectedShapeIndex = row;
    GLShape* shape = glScene->getSelectedShape();
    if (shape) {
        // Block signals to prevent triggering updates while setting values
        rotationXSpin->blockSignals(true);
        rotationYSpin->blockSignals(true);
        rotationZSpin->blockSignals(true);
        translateXSpin->blockSignals(true);
        translateYSpin->blockSignals(true);
        translateZSpin->blockSignals(true);
        scaleSpin->blockSignals(true);

        // Update the spin boxes with the selected shape's properties
        rotationXSpin->setValue(shape->rotationX);
        rotationYSpin->setValue(shape->rotationY);
        rotationZSpin->setValue(shape->rotationZ);
        translateXSpin->setValue(shape->translateX);
        translateYSpin->setValue(shape->translateY);
        translateZSpin->setValue(shape->translateZ);
        scaleSpin->setValue(shape->scale);

        // Unblock signals
        rotationXSpin->blockSignals(false);
        rotationYSpin->blockSignals(false);
        rotationZSpin->blockSignals(false);
        translateXSpin->blockSignals(false);
        translateYSpin->blockSignals(false);
        translateZSpin->blockSignals(false);
        scaleSpin->blockSignals(false);
    }
}

// Slot: Updates the selected shape's transformations when a spin box value changes
void MainWindow::onTransformChanged() {
    GLShape* shape = glScene->getSelectedShape();
    if (shape) {
        // Update the shape's properties with the spin box values
        shape->rotationX = rotationXSpin->value();
        shape->rotationY = rotationYSpin->value();
        shape->rotationZ = rotationZSpin->value();
        shape->translateX = translateXSpin->value();
        shape->translateY = translateYSpin->value();
        shape->translateZ = translateZSpin->value();
        shape->scale = scaleSpin->value();
        glScene->update(); // Redraw the scene
    }
}

// Slot: Deletes the currently selected shape
void MainWindow::onDeleteShape() {
    int row = shapeListWidget->currentRow();
    if (row >= 0) {
        delete shapeListWidget->takeItem(row); // Remove the shape from the list
        glScene->deleteShape(row); // Remove the shape from the GLScene
    }
}
