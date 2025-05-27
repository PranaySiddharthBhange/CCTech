#include "revolve.h"
#include <QtMath>

RevolveWidget::RevolveWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void RevolveWidget::addPoint(const QVector3D &point)
{
    points.append(point);
    update();
}

void RevolveWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPointSize(10.0f);
    glLineWidth(5.0f);
}
void RevolveWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void RevolveWidget::performRevolution()
{
    revolutionTriggered = true;
    update();
}

void RevolveWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        float x = event->position().x();
        float y = event->position().y();
        float glX = 2.0f * x / width() - 1.0f;
        float glY = 1.0f - 2.0f * y / height(); // invert Y axis
        addPoint(QVector3D(glX, glY, 0.0f));
    }
    else if (event->button() == Qt::RightButton)
    {
        performRevolution();
    }
}

void RevolveWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw control points
    glPointSize(6.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_POINTS);
    for (const auto &point : points)
    {
        qDebug() << "Control Point:" << point;
        glVertex3f(point.x(), point.y(), point.z());
    }
    glEnd();

    if (points.size() >= 2)
    {
        QVector<QVector3D> bezierPoints;

        // De Casteljau's algorithm
        for (float t = 0; t <= 1.0f; t += 0.01f)
        {
            QVector<QVector3D> tempPoints = points;
            int n = tempPoints.size();

            while (n > 1)
            {
                for (int i = 0; i < n - 1; ++i)
                {
                    tempPoints[i] = (1 - t) * tempPoints[i] + t * tempPoints[i + 1];
                }
                --n;
            }

            bezierPoints.push_back(tempPoints[0]);
        }

        // Draw Bézier curve
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glBegin(GL_LINE_STRIP);
        for (const auto &bp : bezierPoints)
        {
            glVertex3f(bp.x(), bp.y(), bp.z());
        }
        glEnd();

        // Draw revolution surface if triggered
        if (revolutionTriggered)
        {
            qDebug() << "Drawing revolution with" << bezierPoints.size() << "curve points.";

            const int slices = 36;
            const float angleStep = 360.0f / slices;

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
            glBegin(GL_TRIANGLES);

            for (int i = 0; i < slices; ++i)
            {
                float angle1 = qDegreesToRadians(i * angleStep);
                float angle2 = qDegreesToRadians((i + 1) * angleStep);

                for (int j = 0; j < bezierPoints.size() - 1; ++j)
                {
                    QVector3D p1 = bezierPoints[j];
                    QVector3D p2 = bezierPoints[j + 1];

                    QVector3D p1_rot1(p1.x() * cos(angle1), p1.y(), p1.x() * sin(angle1));
                    QVector3D p1_rot2(p1.x() * cos(angle2), p1.y(), p1.x() * sin(angle2));
                    QVector3D p2_rot1(p2.x() * cos(angle1), p2.y(), p2.x() * sin(angle1));
                    QVector3D p2_rot2(p2.x() * cos(angle2), p2.y(), p2.x() * sin(angle2));

                    glVertex3f(p1_rot1.x(), p1_rot1.y(), p1_rot1.z());
                    glVertex3f(p2_rot1.x(), p2_rot1.y(), p2_rot1.z());
                    glVertex3f(p1_rot2.x(), p1_rot2.y(), p1_rot2.z());

                    glVertex3f(p1_rot2.x(), p1_rot2.y(), p1_rot2.z());
                    glVertex3f(p2_rot1.x(), p2_rot1.y(), p2_rot1.z());
                    glVertex3f(p2_rot2.x(), p2_rot2.y(), p2_rot2.z());
                }
            }

            glEnd();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restore fill mode
        }
    }
}
