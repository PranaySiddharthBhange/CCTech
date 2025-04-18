#ifndef GLSCENE_H
#define GLSCENE_H
#include <QOpenGLWidget>              // Base class for OpenGL rendering in a Qt widget
#include <QOpenGLFunctions_3_3_Core>  // Provides access to OpenGL 3.3 core functions
#include <QMatrix4x4>                 // Used for 4x4 transformation matrices
#include <QOpenGLShaderProgram>       // Manages OpenGL shader programs
#include <QQuaternion>                // Represents rotations in 3D space
#include <QPoint>                     // Represents 2D points
#include <vector>                     // Standard C++ container for storing shapes
#include <QVector3D>                  // Represents 3D vectors
#include <QWheelEvent>                // Handles mouse wheel events

// Enum representing the types of 3D shapes that can be rendered
enum ShapeType { Cube, Sphere, Cylinder, Cone, Custom};

// Struct representing a 3D shape with its properties
struct GLShape {
    ShapeType type;                   // Type of the shape (e.g., Cube, Sphere)
    float rotationX, rotationY, rotationZ; // Rotation angles around X, Y, Z axes
    float translateX, translateY, translateZ; // Translation along X, Y, Z axes
    float scale;                      // Uniform scaling factor


    // For custom shapes only
    GLuint vao = 0;
    GLuint vbo = 0;
    int vertexCount = 0;


    // Constructor to initialize a shape with default transformations
    GLShape(ShapeType t)
        : type(t),
        rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f),
        translateX(0.0f), translateY(0.0f), translateZ(-5.0f),
        scale(1.0f)

    {
    }
};

// Class responsible for rendering 3D shapes and handling user interactions
class GLScene : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT // Macro enabling Qt's signal and slot mechanism

public:
    // Constructor: Initializes the scene with an optional parent widget
    explicit GLScene(QWidget* parent = nullptr);

    // Destructor: Cleans up resources
    ~GLScene();

    // List of shapes in the scene
    std::vector<GLShape> shapes;

    // Index of the currently selected shape (-1 if no shape is selected)
    int selectedShapeIndex;

    // Adds a new shape to the scene
    void addShape(ShapeType type);

    // Deletes a shape from the scene by its index
    void deleteShape(int index);

    // Returns a pointer to the currently selected shape
    GLShape* getSelectedShape();

	GLShape setupCustomGeometry(const std::string& filename);

    //void drawCustomGeometry();
   

protected:
    // Initializes OpenGL resources (e.g., shaders, buffers)
    void initializeGL() override;

    // Handles resizing of the OpenGL viewport
    void resizeGL(int w, int h) override;

    // Renders the scene
    void paintGL() override;

    // Handles mouse press events for interaction
    void mousePressEvent(QMouseEvent* event) override;

    // Handles mouse movement events for interaction
    void mouseMoveEvent(QMouseEvent* event) override;

    // Handles mouse wheel events for zooming
    void wheelEvent(QWheelEvent* event) override;

   

private:
    // Sets up the shader program
    void setupShaders();

    // Sets up geometry for a cube
    void setupCubeGeometry();

    // Sets up geometry for a sphere
    void setupSphereGeometry();

    // Sets up geometry for a cylinder
    void setupCylinderGeometry();

    // Sets up geometry for a cone
    void setupConeGeometry();

    // Draws a cube
    void drawCubeGeometry();

    // Draws a sphere
    void drawSphereGeometry();

    // Draws a cylinder
    void drawCylinderGeometry();

    // Draws a cone
    void drawConeGeometry();

    // Shader program for rendering
    QOpenGLShaderProgram shaderProgram;

    // OpenGL objects for cube geometry
    GLuint vaoCube, vboCube; // Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    int cubeVertexCount;     // Number of vertices in the cube

    // OpenGL objects for sphere geometry
    GLuint vaoSphere, vboSphere;
    int sphereVertexCount;

    // OpenGL objects for cylinder geometry
    GLuint vaoCylinder, vboCylinder;
    int cylinderVertexCount;

    // OpenGL objects for cone geometry
    GLuint vaoCone, vboCone;
    int coneVertexCount;


    // Projection matrix for 3D rendering
    QMatrix4x4 projection;

    // Last mouse position for tracking mouse movements
    QPoint lastMousePosition;

    // Camera properties for viewing the scene
    float cameraDistance;    // Distance of the camera from the scene
    float cameraAzimuth;     // Azimuth angle of the camera
    float cameraElevation;   // Elevation angle of the camera
};

#endif // GLSCENE_H