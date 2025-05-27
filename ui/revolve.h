#ifndef REVOLVE_H
#define REVOLVE_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QVector3D>
#include <QVector>
#include <QDebug>
#include <QOpenGLFunctions>

class RevolveWidget : public QOpenGLWidget,protected QOpenGLFunctions{
    Q_OBJECT

public:
    RevolveWidget(QWidget *parent = nullptr);
    void addPoint(const QVector3D &point);
    void performRevolution();
    QVector<QVector3D> points; 

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void initializeGL() override;

private:
    bool revolutionTriggered = false;
};

#endif // REVOLVE_H
