#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QListWidget;
class QDoubleSpinBox;
class GLScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddShape();
    void onShapeSelected(int row);
    void onTransformChanged();
    void onDeleteShape();

private:
    GLScene* glScene;

    QComboBox* shapeComboBox;
    QListWidget* shapeListWidget;

    QDoubleSpinBox* rotationXSpin, * rotationYSpin, * rotationZSpin;
    QDoubleSpinBox* translateXSpin, * translateYSpin, * translateZSpin;
    QDoubleSpinBox* scaleSpin;
};

#endif
