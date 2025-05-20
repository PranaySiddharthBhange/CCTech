#pragma once

#include <QOpenGLWidget>
#include <QVector>
#include <QPointF>
#include <QOpenGLFunctions>

class BezierWidget : public QOpenGLWidget ,protected QOpenGLFunctions {
    Q_OBJECT

public:
    BezierWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;


private:
    QVector<QPointF> controlPoints;
    QVector<QPointF> bezierCurve;

    void computeBezierCurve();
    QPointF deCasteljau(float t);
};
