#include "mainwindow.h"
#include "glscene.h"
#include "bezierwindow.h"

#include "revolve.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    glScene = new GLScene(this);
    setCentralWidget(glScene);

    QWidget *controlPanel = new QWidget(this);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);

    shapeComboBox = new QComboBox(controlPanel);
    shapeComboBox->addItem("Cube", QVariant::fromValue(Cube));
    shapeComboBox->addItem("Sphere", QVariant::fromValue(Sphere));
    shapeComboBox->addItem("Cylinder", QVariant::fromValue(Cylinder));
    shapeComboBox->addItem("Cone", QVariant::fromValue(Cone));
    shapeComboBox->addItem("Custom", QVariant::fromValue(Custom));
    controlLayout->addWidget(shapeComboBox);

    QPushButton *addShapeButton = new QPushButton("Add Shape", controlPanel);
    controlLayout->addWidget(addShapeButton);
    connect(addShapeButton, &QPushButton::clicked, this, &MainWindow::onAddShape);

    QPushButton *Bezier = new QPushButton("Bezier", controlPanel);
    controlLayout->addWidget(Bezier);
    connect(Bezier, &QPushButton::clicked, this, &MainWindow::onBezierButtonClicked);

    QPushButton *revolve = new QPushButton("Revolve", controlPanel);
    controlLayout->addWidget(revolve);
    connect(revolve, &QPushButton::clicked, this, &MainWindow::onRevolveButtonClicked);

    shapeListWidget = new QListWidget(controlPanel);
    controlLayout->addWidget(shapeListWidget);
    connect(shapeListWidget, &QListWidget::currentRowChanged, this, &MainWindow::onShapeSelected);

    QGroupBox *transformGroup = new QGroupBox("Transformations", controlPanel);
    QFormLayout *formLayout = new QFormLayout(transformGroup);

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
    translateZSpin->setValue(-5);
    formLayout->addRow("Translate Z:", translateZSpin);
    connect(translateZSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    scaleSpin = new QDoubleSpinBox(transformGroup);
    scaleSpin->setRange(0.1, 10);
    scaleSpin->setSingleStep(0.1);
    formLayout->addRow("Scale:", scaleSpin);
    connect(scaleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onTransformChanged);

    controlLayout->addWidget(transformGroup);

    QPushButton *deleteShapeButton = new QPushButton("Delete Shape", controlPanel);
    controlLayout->addWidget(deleteShapeButton);
    connect(deleteShapeButton, &QPushButton::clicked, this, &MainWindow::onDeleteShape);

    controlPanel->setLayout(controlLayout);
    QDockWidget *dock = new QDockWidget("Controls", this);
    dock->setWidget(controlPanel);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::onAddShape()
{
    ShapeType type = static_cast<ShapeType>(shapeComboBox->currentData().toInt());

    if (type == ShapeType::Custom)
    {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open 3D Model File"), "", tr("3D Model Files (*.stl *.obj *.txt);;All Files (*)"));
        if (filePath.isEmpty())
        {
            return;
        }
        glScene->makeCurrent();
        GLShape shape = glScene->setupCustomGeometry(filePath.toStdString());
        glScene->shapes.push_back(shape);
        glScene->selectedShapeIndex = glScene->shapes.size() - 1;
        glScene->doneCurrent();
        shapeListWidget->addItem(QFileInfo(filePath).fileName());
    }
    else
    {
        glScene->addShape(type);
        shapeListWidget->addItem(shapeComboBox->currentText());
    }

    shapeListWidget->setCurrentRow(static_cast<int>(glScene->shapes.size()) - 1);
    glScene->update();
}

void MainWindow::onShapeSelected(int row)
{
    glScene->selectedShapeIndex = row;
    GLShape *shape = glScene->getSelectedShape();
    if (shape)
    {
        rotationXSpin->blockSignals(true);
        rotationYSpin->blockSignals(true);
        rotationZSpin->blockSignals(true);
        translateXSpin->blockSignals(true);
        translateYSpin->blockSignals(true);
        translateZSpin->blockSignals(true);
        scaleSpin->blockSignals(true);

        rotationXSpin->setValue(shape->rotationX);
        rotationYSpin->setValue(shape->rotationY);
        rotationZSpin->setValue(shape->rotationZ);
        translateXSpin->setValue(shape->translateX);
        translateYSpin->setValue(shape->translateY);
        translateZSpin->setValue(shape->translateZ);
        scaleSpin->setValue(shape->scale);

        rotationXSpin->blockSignals(false);
        rotationYSpin->blockSignals(false);
        rotationZSpin->blockSignals(false);
        translateXSpin->blockSignals(false);
        translateYSpin->blockSignals(false);
        translateZSpin->blockSignals(false);
        scaleSpin->blockSignals(false);
    }
}

void MainWindow::onTransformChanged()
{
    GLShape *shape = glScene->getSelectedShape();
    if (shape)
    {
        shape->rotationX = rotationXSpin->value();
        shape->rotationY = rotationYSpin->value();
        shape->rotationZ = rotationZSpin->value();
        shape->translateX = translateXSpin->value();
        shape->translateY = translateYSpin->value();
        shape->translateZ = translateZSpin->value();
        shape->scale = scaleSpin->value();
        glScene->update();
    }
}

void MainWindow::onDeleteShape()
{
    int row = shapeListWidget->currentRow();
    if (row >= 0)
    {
        delete shapeListWidget->takeItem(row);
        glScene->deleteShape(row);
    }
}

void MainWindow::onBezierButtonClicked()
{
    bezierWidget = new BezierWidget(this);
    bezierWidget->setMinimumSize(400, 400);
    QDockWidget *bezierDock = new QDockWidget("Bezier Curve", this);
    bezierDock->setWidget(bezierWidget);
    addDockWidget(Qt::BottomDockWidgetArea, bezierDock);
    bezierDock->setFloating(true);
}

void MainWindow::onRevolveButtonClicked()
{
    revolveWidget = new RevolveWidget(this);
    revolveWidget->setMinimumSize(400, 400);
    QDockWidget *revolveDock = new QDockWidget("Revolve", this);
    revolveDock->setWidget(revolveWidget);
    addDockWidget(Qt::BottomDockWidgetArea, revolveDock);
    revolveDock->setFloating(true);
}
