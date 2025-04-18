#include "glscene.h"
#include <QTimer>
#include <cmath>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <vector>
#include <QTextStream>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <QMessageBox>

// Constructor: Initializes the GLScene and sets up a timer for continuous updates
GLScene::GLScene(QWidget* parent)
    : QOpenGLWidget(parent),
    selectedShapeIndex(-1),
    vaoCube(0), vboCube(0), cubeVertexCount(0),
    vaoSphere(0), vboSphere(0), sphereVertexCount(0),
    vaoCylinder(0), vboCylinder(0), cylinderVertexCount(0),
    vaoCone(0), vboCone(0), coneVertexCount(0),
    cameraDistance(10.0f),
    cameraAzimuth(0.0f),
    cameraElevation(0.0f)
{
    // Timer to refresh the scene at ~60 FPS (16 ms interval)
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&GLScene::update));
    timer->start(16);
}

// Destructor: Cleans up OpenGL resources
GLScene::~GLScene() {
    makeCurrent(); // Make the OpenGL context current
    glDeleteVertexArrays(1, &vaoCube);
    glDeleteBuffers(1, &vboCube);
    glDeleteVertexArrays(1, &vaoSphere);
    glDeleteBuffers(1, &vboSphere);
    glDeleteVertexArrays(1, &vaoCylinder);
    glDeleteBuffers(1, &vboCylinder);
    glDeleteVertexArrays(1, &vaoCone);
    glDeleteBuffers(1, &vboCone);
    for (auto& shape : shapes) {
        if (shape.type == Custom) {
            glDeleteVertexArrays(1, &shape.vao);
            glDeleteBuffers(1, &shape.vbo);
        }
    }

	// Clean up shader program
	shaderProgram.removeAllShaders(); // Remove all shaders from the program
	shaderProgram.release(); // Release the shader program
    doneCurrent(); // Release the OpenGL context
}

// Adds a new shape to the scene
void GLScene::addShape(ShapeType type) {
    shapes.push_back(GLShape(type)); // Add the shape to the list
    selectedShapeIndex = shapes.size() - 1; // Select the newly added shape
    update(); // Redraw the scene
}

// Deletes a shape from the scene by its index
void GLScene::deleteShape(int index) {
    if (index >= 0 && index < static_cast<int>(shapes.size())) {
        shapes.erase(shapes.begin() + index); // Remove the shape from the list
        if (selectedShapeIndex >= static_cast<int>(shapes.size()))
            selectedShapeIndex = shapes.size() - 1; // Update the selected shape index
        update(); // Redraw the scene
    }
}

// Returns a pointer to the currently selected shape
GLShape* GLScene::getSelectedShape() {
    if (selectedShapeIndex >= 0 && selectedShapeIndex < static_cast<int>(shapes.size()))
        return &shapes[selectedShapeIndex];
    return nullptr; // Return nullptr if no shape is selected
}

// Initializes OpenGL resources
void GLScene::initializeGL() {
    initializeOpenGLFunctions(); // Initialize OpenGL functions
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Set the background color
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode for debugging

    setupShaders(); // Set up the shader program
    setupCubeGeometry(); // Set up geometry for a cube
    setupSphereGeometry(); // Set up geometry for a sphere
    setupCylinderGeometry(); // Set up geometry for a cylinder
    setupConeGeometry();// Set up geometry for a cone   
}

// Handles resizing of the OpenGL viewport
void GLScene::resizeGL(int w, int h) {
    glViewport(0, 0, w, h); // Set the viewport to the new window size
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / h, 0.1f, 1000.0f); // Set up a perspective projection
}

// Renders the scene
void GLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and depth buffer
    shaderProgram.bind(); // Bind the shader program
    // Calculate the camera position based on azimuth and elevation
    float radAzimuth = qDegreesToRadians(cameraAzimuth);
    float radElevation = qDegreesToRadians(cameraElevation);
    QVector3D cameraPos;
    cameraPos.setX(cameraDistance * cos(radElevation) * sin(radAzimuth));
    cameraPos.setY(cameraDistance * sin(radElevation));
    cameraPos.setZ(cameraDistance * cos(radElevation) * cos(radAzimuth));
    QMatrix4x4 view;
    view.lookAt(cameraPos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)); // Set up the view matrix
    // Render each shape in the scene
    for (auto& shape : shapes) {
        QMatrix4x4 model;
        model.translate(shape.translateX, shape.translateY, shape.translateZ); // Apply translation
        model.scale(shape.scale); // Apply scaling
        model.rotate(shape.rotationX, 1.0f, 0.0f, 0.0f); // Apply rotation around X-axis
        model.rotate(shape.rotationY, 0.0f, 1.0f, 0.0f); // Apply rotation around Y-axis
        model.rotate(shape.rotationZ, 0.0f, 0.0f, 1.0f); // Apply rotation around Z-axis

        shaderProgram.setUniformValue("model", model);
        shaderProgram.setUniformValue("view", view);
        shaderProgram.setUniformValue("projection", projection);

        // Draw the shape based on its type
        if (shape.type == Cube)
            drawCubeGeometry();
        else if (shape.type == Sphere)
            drawSphereGeometry();
        else if (shape.type == Cone)
            drawConeGeometry();
        else if (shape.type == Cylinder)
            drawCylinderGeometry();
		else if (shape.type == Custom)
        {
            glBindVertexArray(shape.vao);
            glDrawArrays(GL_TRIANGLES, 0, shape.vertexCount);
            glBindVertexArray(0);
            //drawCustomGeometry();
        }

    }
    shaderProgram.release(); // Release the shader program
}
// Handles mouse press events
void GLScene::mousePressEvent(QMouseEvent* event) {
    lastMousePosition = event->pos(); // Store the mouse position
}
// Handles mouse movement events for camera rotation
void GLScene::mouseMoveEvent(QMouseEvent* event) {
    QPoint delta = event->pos() - lastMousePosition; // Calculate the mouse movement delta
    lastMousePosition = event->pos(); // Update the last mouse position

    float sensitivity = 0.5f; // Adjust sensitivity as needed
    cameraAzimuth += delta.x() * sensitivity; // Update azimuth based on horizontal movement
    cameraElevation += delta.y() * sensitivity; // Update elevation based on vertical movement

    // Clamp the elevation to avoid flipping
    if (cameraElevation > 89.0f)  cameraElevation = 89.0f;
    if (cameraElevation < -89.0f) cameraElevation = -89.0f;

    update(); // Redraw the scene
}
// Handles mouse wheel events for zooming
void GLScene::wheelEvent(QWheelEvent* event) {
    float delta = event->angleDelta().y() / 120.0f; // One notch is typically 120
    cameraDistance -= delta; // Subtract to zoom in on wheel up
    cameraDistance = qBound(2.0f, cameraDistance, 100.0f); // Clamp the camera distance
    update(); // Redraw the scene
}
// Sets up the shader program
void GLScene::setupShaders() {
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.glsl"))
        qWarning("Failed to compile vertex shader.");
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragment.glsl"))
        qWarning("Failed to compile fragment shader.");
    if (!shaderProgram.link())
        qWarning("Failed to link shader program.");
}
void GLScene::setupCubeGeometry() {
    // Define a cube with 36 vertices (12 triangles)
    float vertices[] = {
        // Front face
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        // Back face
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        // Left face
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        // Right face
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         // Top face
         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,
         // Bottom face
         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f
    };
    cubeVertexCount = 36; // Number of vertices
    glGenVertexArrays(1, &vaoCube);
    glGenBuffers(1, &vboCube);
    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void GLScene::drawCubeGeometry() {
    glBindVertexArray(vaoCube);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
    glBindVertexArray(0);
}
void GLScene::setupSphereGeometry() {
    std::vector<float> vertices; // Stores the vertex data for the sphere
    const float radius = 1.0f;   // Radius of the sphere
    const int sectors = 20;      // Number of vertical divisions (longitude)
    const int stacks = 20;       // Number of horizontal divisions (latitude)

    // Loop through each stack (latitude)
    for (int i = 0; i < stacks; ++i) {
        float stackAngle1 = M_PI / 2 - i * M_PI / stacks;       // Starting angle of the stack
        float stackAngle2 = M_PI / 2 - (i + 1) * M_PI / stacks; // Ending angle of the stack
        float y1 = radius * sin(stackAngle1); // Y-coordinate of the first stack
        float y2 = radius * sin(stackAngle2); // Y-coordinate of the second stack
        float r1 = radius * cos(stackAngle1); // Radius of the first stack
        float r2 = radius * cos(stackAngle2); // Radius of the second stack

        // Loop through each sector (longitude)
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * M_PI / sectors; // Angle of the sector
            float x1 = r1 * cos(sectorAngle); // X-coordinate of the first stack
            float z1 = r1 * sin(sectorAngle); // Z-coordinate of the first stack
            float x2 = r2 * cos(sectorAngle); // X-coordinate of the second stack
            float z2 = r2 * sin(sectorAngle); // Z-coordinate of the second stack

            // Add two vertices for the quad strip
            vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(z1);
            vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
        }
    }

    sphereVertexCount = vertices.size() / 3; // Total number of vertices
    glGenVertexArrays(1, &vaoSphere);
    glGenBuffers(1, &vboSphere);
    glBindVertexArray(vaoSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void GLScene::drawSphereGeometry() {
    glBindVertexArray(vaoSphere); // Bind the sphere's VAO
    int sectors = 20;             // Number of vertical divisions (longitude)
    int verticesPerStrip = (sectors + 1) * 2; // Number of vertices per triangle strip

    // Draw each stack as a triangle strip
    for (int i = 0; i < 20; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * verticesPerStrip, verticesPerStrip);
    }
    glBindVertexArray(0); // Unbind the VAO
}
void GLScene::setupCylinderGeometry() {
    std::vector<float> vertices; // Stores the vertex data for the cylinder
    const float radius = 1.0f;   // Radius of the cylinder
    const float height = 2.0f;   // Height of the cylinder
    const int slices = 20;       // Number of divisions around the circumference

    // Generate the side surface (quad strip)
    for (int i = 0; i <= slices; i++) {
        float theta = 2.0f * M_PI * i / slices; // Angle of the slice
        float x = cos(theta); // X-coordinate
        float z = sin(theta); // Z-coordinate

        // Add bottom and top vertices for the quad strip
        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f); // Bottom vertex
        vertices.push_back(radius * z);
        vertices.push_back(radius * x);
        vertices.push_back(height / 2.0f);  // Top vertex
        vertices.push_back(radius * z);
    }

    // Generate the top circle (triangle fan)
    vertices.push_back(0.0f); vertices.push_back(height / 2.0f); vertices.push_back(0.0f); // Center vertex
    for (int i = 0; i <= slices; i++) {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(radius * x);
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * z);
    }

    // Generate the bottom circle (triangle fan)
    vertices.push_back(0.0f); vertices.push_back(-height / 2.0f); vertices.push_back(0.0f); // Center vertex
    for (int i = 0; i <= slices; i++) {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * z);
    }

    cylinderVertexCount = vertices.size() / 3; // Total number of vertices
    glGenVertexArrays(1, &vaoCylinder);
    glGenBuffers(1, &vboCylinder);
    glBindVertexArray(vaoCylinder);
    glBindBuffer(GL_ARRAY_BUFFER, vboCylinder);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void GLScene::drawCylinderGeometry() {
    glBindVertexArray(vaoCylinder); // Bind the cylinder's VAO
    int slices = 20;                // Number of divisions around the circumference
    int sideVertices = (slices + 1) * 2; // Number of vertices for the side surface

    // Draw the side surface
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sideVertices);

    // Draw the top circle
    glDrawArrays(GL_TRIANGLE_FAN, sideVertices, slices + 2);

    // Draw the bottom circle
    glDrawArrays(GL_TRIANGLE_FAN, sideVertices + (slices + 2), slices + 2);

    glBindVertexArray(0); // Unbind the VAO
}
void GLScene::setupConeGeometry() {
    std::vector<float> vertices; // Stores the vertex data for the cone
    const float radius = 1.0f;   // Radius of the base
    const float height = 2.0f;   // Height of the cone
    const int slices = 20;       // Number of divisions around the circumference

    // Generate the side surface (triangle fan)
    for (int i = 0; i <= slices; i++) {
        float theta = 2.0f * M_PI * i / slices; // Angle of the slice
        float x = cos(theta); // X-coordinate
        float z = sin(theta); // Z-coordinate

        // Add bottom vertex and tip vertex
        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f); // Bottom vertex
        vertices.push_back(radius * z);
        vertices.push_back(0.0f);
        vertices.push_back(height / 2.0f);  // Tip vertex
        vertices.push_back(0.0f);
    }

    // Generate the bottom circle (triangle fan)
    vertices.push_back(0.0f); vertices.push_back(-height / 2.0f); vertices.push_back(0.0f); // Center vertex
    for (int i = 0; i <= slices; i++) {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * z);
    }

    coneVertexCount = vertices.size() / 3; // Total number of vertices
    glGenVertexArrays(1, &vaoCone);
    glGenBuffers(1, &vboCone);
    glBindVertexArray(vaoCone);
    glBindBuffer(GL_ARRAY_BUFFER, vboCone);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void GLScene::drawConeGeometry() {
    glBindVertexArray(vaoCone); // Bind the cone's VAO
    int slices = 20;            // Number of divisions around the circumference
    int sideVertices = (slices + 1) * 2; // Number of vertices for the side surface

    // Draw the side surface
    glDrawArrays(GL_TRIANGLE_FAN, 0, sideVertices);

    // Draw the bottom circle
    glDrawArrays(GL_TRIANGLE_FAN, sideVertices, slices + 2);

    glBindVertexArray(0); // Unbind the VAO
}
GLShape GLScene::setupCustomGeometry(const std::string& filename) {
    GLShape shape(Custom);  // Initialize shape

    struct Vec3 {
        float x, y, z;
    };
    struct Face {
        std::vector<int> vertices;
    };
    std::vector<float> vertices;

    // Detect file extension
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        std::cerr << "Error: No file extension found.\n";
        return shape;  // Return default shape
    }

    std::string extension = filename.substr(dotPos + 1);

    if (extension == "txt") {
        std::ifstream inputFile(filename);
        std::string line;

        if (inputFile.is_open()) {
            while (std::getline(inputFile, line)) {
                size_t first = line.find_first_not_of(" \t\n\r");
                if (first == std::string::npos) continue;
                size_t last = line.find_last_not_of(" \t\n\r");
                line = line.substr(first, last - first + 1);

                std::istringstream iss(line);
                float x, y, z;
                if (iss >> x >> y >> z) {
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                }
                else {
                    std::cerr << "Warning: Invalid line skipped: " << line << std::endl;
                }
            }
            inputFile.close();
        }
        else {
            std::cerr << "Error: Could not open .txt file.\n";
            return shape;  // Return default shape on error
        }
    }
    else if (extension == "stl") {
        struct Triangle {
            float vertices[3][3];
        };
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open STL file.\n";
            return shape;  // Return default shape on error
        }

        char header[5] = { 0 };
        file.read(header, 5);
        file.seekg(0, std::ios::beg);
        bool isASCII = std::strncmp(header, "solid", 5) == 0;
        std::vector<Triangle> triangles;

        if (isASCII) {
            std::string line;
            Triangle tri;
            int vertexIndex = 0;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string word;
                ss >> word;
                if (word == "vertex") {
                    ss >> tri.vertices[vertexIndex][0]
                        >> tri.vertices[vertexIndex][1]
                        >> tri.vertices[vertexIndex][2];
                    vertexIndex++;
                    if (vertexIndex == 3) {
                        triangles.push_back(tri);
                        vertexIndex = 0;
                    }
                }
            }
        }
        else {
            char binHeader[80];
            file.read(binHeader, 80);
            uint32_t numTriangles;
            file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));
            triangles.reserve(numTriangles);

            for (uint32_t i = 0; i < numTriangles; ++i) {
                Triangle tri;
                float normal[3];
                file.read(reinterpret_cast<char*>(normal), sizeof(normal));
                for (int v = 0; v < 3; ++v) {
                    file.read(reinterpret_cast<char*>(&tri.vertices[v][0]), sizeof(float) * 3);
                }
                uint16_t attribute;
                file.read(reinterpret_cast<char*>(&attribute), sizeof(attribute));
                triangles.push_back(tri);
            }
        }

        file.close();

        for (const auto& tri : triangles) {
            for (int v = 0; v < 3; ++v) {
                vertices.push_back(tri.vertices[v][0]);
                vertices.push_back(tri.vertices[v][1]);
                vertices.push_back(tri.vertices[v][2]);
            }
        }
    }
    else if (extension == "obj") {
        std::ifstream objStream(filename);
        if (!objStream.is_open()) {
            std::cerr << "Failed to open OBJ file: " << filename << std::endl;
            return shape;  // Return default shape on error
        }

        std::vector<Vec3> vertexList;
        std::vector<Face> faceList;
        std::string line;

        while (std::getline(objStream, line)) {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v") {
                Vec3 v;
                ss >> v.x >> v.y >> v.z;
                vertexList.push_back(v);
            }
            else if (prefix == "f") {
                Face face;
                std::string vtn;
                while (ss >> vtn) {
                    size_t slashPos = vtn.find('/');
                    int vIndex = (slashPos != std::string::npos) ? std::stoi(vtn.substr(0, slashPos)) - 1 : std::stoi(vtn) - 1;
                    face.vertices.push_back(vIndex);
                }
                faceList.push_back(face);
            }
        }

        objStream.close();

        for (const auto& face : faceList) {
            if (face.vertices.size() < 3) continue;
            for (size_t i = 1; i + 1 < face.vertices.size(); ++i) {
                Vec3 v0 = vertexList[face.vertices[0]];
                Vec3 v1 = vertexList[face.vertices[i]];
                Vec3 v2 = vertexList[face.vertices[i + 1]];

                vertices.insert(vertices.end(), { v0.x, v0.y, v0.z,
                                                  v1.x, v1.y, v1.z,
                                                  v2.x, v2.y, v2.z });
            }
        }
    }
    else {
        std::cerr << "Unsupported file extension: " << extension << std::endl;
        return shape;  // Return default shape on unsupported file type
    }

    shape.vertexCount = vertices.size() / 3;
    if (shape.vertexCount > 0) {
        glGenVertexArrays(1, &shape.vao);
        glGenBuffers(1, &shape.vbo);
        glBindVertexArray(shape.vao);
        glBindBuffer(GL_ARRAY_BUFFER, shape.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    update();  // Assuming this is a function to refresh your OpenGL context

    return shape;  // Always return a shape, even if it's empty or an error
}

