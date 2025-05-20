#include "bezierwindow.h"
#include <QMouseEvent>
#include <QOpenGLFunctions>

BezierWidget::BezierWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void BezierWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPointSize(10.0f);
    glLineWidth(5.0f);
}

void BezierWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void BezierWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINE_STRIP);
    for (const auto &pt : controlPoints)
        glVertex2f(pt.x(), pt.y());
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto &pt : bezierCurve)
        glVertex2f(pt.x(), pt.y());
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    for (const auto &pt : controlPoints)
        glVertex2f(pt.x(), pt.y());
    glEnd();
}

void BezierWidget::mousePressEvent(QMouseEvent *event)
{
    int index = 0;

    if (event->button() == Qt::LeftButton)
    {
        qDebug() << "Left Button Clicked";
        controlPoints.append(event->pos());
    }
    else 
    {
        int ld = INT16_MAX;
       
        for(int i=0;i<controlPoints.size();i++){

            qDebug() << "Entered Loop";
            auto x =event->pos().x();
            qDebug() << x;
            auto y =event->pos().y();
            qDebug() << y;
            auto x2= controlPoints[i].x();
            qDebug() << x2;
            auto y2= controlPoints[i].y();
            qDebug() << y2;
            auto d = sqrt(pow((x2-x),2) + pow((y2-y),2));

            qDebug()<< "Distance is "<<d;

            if(d<ld){
                ld=d;
                index=i;
            }
        }

        controlPoints[index] = event->pos();

        
    }


    computeBezierCurve();
    update();
}

void BezierWidget::computeBezierCurve()
{
    bezierCurve.clear();
    if (controlPoints.size() < 2)
        return;

    for (float t = 0.0f; t <= 1.0f; t += 0.01f)
        bezierCurve.append(deCasteljau(t));
}

QPointF BezierWidget::deCasteljau(float t)
{
    QVector<QPointF> pts = controlPoints;
    while (pts.size() > 1)
    {
        QVector<QPointF> next;
        for (int i = 0; i < pts.size() - 1; ++i)
        {
            float x = (1 - t) * pts[i].x() + t * pts[i + 1].x();
            float y = (1 - t) * pts[i].y() + t * pts[i + 1].y();
            next.append(QPointF(x, y));
        }
        pts = next;
    }
    return pts[0];
}






