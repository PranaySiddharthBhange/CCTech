#ifndef GLSCENE_H
#define GLSCENE_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QQuaternion>
#include <QPoint>
#include <vector>
#include <QVector3D>
#include <QWheelEvent>

enum ShapeType { Cube, Sphere, Cylinder, Cone, Custom};

struct GLShape {
    ShapeType type;
    float rotationX, rotationY, rotationZ;
    float translateX, translateY, translateZ;
    float scale;

    GLuint vao = 0;
    GLuint vbo = 0;
    int vertexCount = 0;

    GLShape(ShapeType t)
        : type(t),
        rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f),
        translateX(0.0f), translateY(0.0f), translateZ(-5.0f),
        scale(1.0f)
    {
    }
};

class GLScene : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLScene(QWidget* parent = nullptr);
    ~GLScene();

    std::vector<GLShape> shapes;
    int selectedShapeIndex;

    void addShape(ShapeType type);
    void deleteShape(int index);
    GLShape* getSelectedShape();
    GLShape setupCustomGeometry(const std::string& filename);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void setupShaders();
    void setupCubeGeometry();
    void setupSphereGeometry();
    void setupCylinderGeometry();
    void setupConeGeometry();
    void drawCubeGeometry();
    void drawSphereGeometry();
    void drawCylinderGeometry();
    void drawConeGeometry();

    QOpenGLShaderProgram shaderProgram;

    GLuint vaoCube, vboCube;
    int cubeVertexCount;

    GLuint vaoSphere, vboSphere;
    int sphereVertexCount;

    GLuint vaoCylinder, vboCylinder;
    int cylinderVertexCount;

    GLuint vaoCone, vboCone;
    int coneVertexCount;

    QMatrix4x4 projection;

    QPoint lastMousePosition;

    float cameraDistance;
    float cameraAzimuth;
    float cameraElevation;
};

#endif
