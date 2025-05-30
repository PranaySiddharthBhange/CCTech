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

GLScene::GLScene(QWidget *parent)
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
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&GLScene::update));
    timer->start(16);
}

GLScene::~GLScene()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vaoCube);
    glDeleteBuffers(1, &vboCube);
    glDeleteVertexArrays(1, &vaoSphere);
    glDeleteBuffers(1, &vboSphere);
    glDeleteVertexArrays(1, &vaoCylinder);
    glDeleteBuffers(1, &vboCylinder);
    glDeleteVertexArrays(1, &vaoCone);
    glDeleteBuffers(1, &vboCone);
    for (auto &shape : shapes)
    {
        if (shape.type == Custom)
        {
            glDeleteVertexArrays(1, &shape.vao);
            glDeleteBuffers(1, &shape.vbo);
        }
    }

    shaderProgram.removeAllShaders();
    shaderProgram.release();
    doneCurrent();
}

void GLScene::addShape(ShapeType type)
{
    shapes.push_back(GLShape(type));
    selectedShapeIndex = shapes.size() - 1;
    update();
}

void GLScene::deleteShape(int index)
{
    if (index >= 0 && index < static_cast<int>(shapes.size()))
    {
        shapes.erase(shapes.begin() + index);
        if (selectedShapeIndex >= static_cast<int>(shapes.size()))
            selectedShapeIndex = shapes.size() - 1;
        update();
    }
}

GLShape *GLScene::getSelectedShape()
{
    if (selectedShapeIndex >= 0 && selectedShapeIndex < static_cast<int>(shapes.size()))
        return &shapes[selectedShapeIndex];
    return nullptr;
}

void GLScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    setupShaders();
    setupCubeGeometry();
    setupSphereGeometry();
    setupCylinderGeometry();
    setupConeGeometry();
}

void GLScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / h, 0.1f, 1000.0f);
}

void GLScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();
    float radAzimuth = qDegreesToRadians(cameraAzimuth);
    float radElevation = qDegreesToRadians(cameraElevation);
    QVector3D cameraPos;
    cameraPos.setX(cameraDistance * cos(radElevation) * sin(radAzimuth));
    cameraPos.setY(cameraDistance * sin(radElevation));
    cameraPos.setZ(cameraDistance * cos(radElevation) * cos(radAzimuth));
    QMatrix4x4 view;
    view.lookAt(cameraPos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    for (auto &shape : shapes)
    {
        QMatrix4x4 model;
        model.translate(shape.translateX, shape.translateY, shape.translateZ);
        model.scale(shape.scale);
        model.rotate(shape.rotationX, 1.0f, 0.0f, 0.0f);
        model.rotate(shape.rotationY, 0.0f, 1.0f, 0.0f);
        model.rotate(shape.rotationZ, 0.0f, 0.0f, 1.0f);

        shaderProgram.setUniformValue("model", model);
        shaderProgram.setUniformValue("view", view);
        shaderProgram.setUniformValue("projection", projection);

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
        }
    }
    shaderProgram.release();
}

void GLScene::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
}

void GLScene::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - lastMousePosition;
    lastMousePosition = event->pos();

    float sensitivity = 0.5f;
    cameraAzimuth += delta.x() * sensitivity;
    cameraElevation += delta.y() * sensitivity;

    if (cameraElevation > 89.0f)
        cameraElevation = 89.0f;
    if (cameraElevation < -89.0f)
        cameraElevation = -89.0f;

    update();
}

void GLScene::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() / 120.0f;
    cameraDistance -= delta;
    cameraDistance = qBound(2.0f, cameraDistance, 100.0f);
    update();
}

void GLScene::setupShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Pranay Bhange\\Desktop\\STUDIO PROJECT\\Sketcher\\ui\\vertex.glsl"))
    {
        qWarning("Failed to compile vertex shader.");
        qDebug() << shaderProgram.log();
    }
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Pranay Bhange\\Desktop\\STUDIO PROJECT\\Sketcher\\ui\\fragment.glsl"))
    {
        qWarning("Failed to compile fragment shader.");
        qDebug() << shaderProgram.log();
    }
    if (!shaderProgram.link())
    {
        qWarning("Failed to link shader program.");
        qDebug() << shaderProgram.log();
    }
}

// void GLScene::setupShaders()
// {
//     const char* vertexShaderSource = R"(#version 330 core
//     layout (location = 0) in vec3 aPos;

//     uniform mat4 model;
//     uniform mat4 view;
//     uniform mat4 projection;

//     void main() {
//         gl_Position = projection * view * model * vec4(aPos, 1.0);
//     })";

//     const char* fragmentShaderSource = R"(#version 330 core
//     out vec4 FragColor;

//     void main() {
//         FragColor = vec4(0.2, 0.7, 0.9, 1.0);
//     })";

//     if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource))
//     {
//         qWarning("Failed to compile vertex shader.");
//         qDebug() << shaderProgram.log();
//     }

//     if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource))
//     {
//         qWarning("Failed to compile fragment shader.");
//         qDebug() << shaderProgram.log();
//     }

//     if (!shaderProgram.link())
//     {
//         qWarning("Failed to link shader program.");
//         qDebug() << shaderProgram.log();
//     }
// }


void GLScene::setupCubeGeometry()
{
    float vertices[] = {
        -1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
    };
    glGenVertexArrays(1, &vaoCube);
    glGenBuffers(1, &vboCube);
    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    cubeVertexCount = sizeof(vertices) / sizeof(vertices[0]) / 3;
}

void GLScene::drawCubeGeometry()
{
    glBindVertexArray(vaoCube);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
    glBindVertexArray(0);
}
void GLScene::setupSphereGeometry()
{
    std::vector<float> vertices;
    const float radius = 1.0f;
    const int sectors = 20;
    const int stacks = 20;

    for (int i = 0; i < stacks; ++i)
    {
        float stackAngle1 = M_PI / 2 - i * M_PI / stacks;
        float stackAngle2 = M_PI / 2 - (i + 1) * M_PI / stacks;
        float y1 = radius * sin(stackAngle1);
        float y2 = radius * sin(stackAngle2);
        float r1 = radius * cos(stackAngle1);
        float r2 = radius * cos(stackAngle2);

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * 2 * M_PI / sectors;
            float x1 = r1 * cos(sectorAngle);
            float z1 = r1 * sin(sectorAngle);
            float x2 = r2 * cos(sectorAngle);
            float z2 = r2 * sin(sectorAngle);

            vertices.push_back(x1);
            vertices.push_back(y1);
            vertices.push_back(z1);
            vertices.push_back(x2);
            vertices.push_back(y2);
            vertices.push_back(z2);
        }
    }

    sphereVertexCount = vertices.size() / 3;
    glGenVertexArrays(1, &vaoSphere);
    glGenBuffers(1, &vboSphere);
    glBindVertexArray(vaoSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void GLScene::drawSphereGeometry()
{
    glBindVertexArray(vaoSphere);
    int sectors = 20;
    int verticesPerStrip = (sectors + 1) * 2;

    for (int i = 0; i < 20; i++)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i * verticesPerStrip, verticesPerStrip);
    }
    glBindVertexArray(0);
}

void GLScene::setupCylinderGeometry()
{
    std::vector<float> vertices;
    const float radius = 1.0f;
    const float height = 2.0f;
    const int slices = 20;

    for (int i = 0; i <= slices; i++)
    {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);

        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * z);
        vertices.push_back(radius * x);
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * z);
    }

    vertices.push_back(0.0f);
    vertices.push_back(height / 2.0f);
    vertices.push_back(0.0f);
    for (int i = 0; i <= slices; i++)
    {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(radius * x);
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * z);
    }

    vertices.push_back(0.0f);
    vertices.push_back(-height / 2.0f);
    vertices.push_back(0.0f);
    for (int i = 0; i <= slices; i++)
    {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * z);
    }

    cylinderVertexCount = vertices.size() / 3;
    glGenVertexArrays(1, &vaoCylinder);
    glGenBuffers(1, &vboCylinder);
    glBindVertexArray(vaoCylinder);
    glBindBuffer(GL_ARRAY_BUFFER, vboCylinder);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void GLScene::drawCylinderGeometry()
{
    glBindVertexArray(vaoCylinder);
    int slices = 20;
    int sideVertices = (slices + 1) * 2;

    glDrawArrays(GL_TRIANGLE_STRIP, 0, sideVertices);
    glDrawArrays(GL_TRIANGLE_FAN, sideVertices, slices + 2);
    glDrawArrays(GL_TRIANGLE_FAN, sideVertices + (slices + 2), slices + 2);

    glBindVertexArray(0);
}

void GLScene::setupConeGeometry()
{
    std::vector<float> vertices;
    const float radius = 1.0f;
    const float height = 2.0f;
    const int slices = 20;

    for (int i = 0; i <= slices; i++)
    {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);

        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * z);
        vertices.push_back(0.0f);
        vertices.push_back(height / 2.0f);
        vertices.push_back(0.0f);
    }

    vertices.push_back(0.0f);
    vertices.push_back(-height / 2.0f);
    vertices.push_back(0.0f);
    for (int i = 0; i <= slices; i++)
    {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(radius * x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * z);
    }

    coneVertexCount = vertices.size() / 3;
    glGenVertexArrays(1, &vaoCone);
    glGenBuffers(1, &vboCone);
    glBindVertexArray(vaoCone);
    glBindBuffer(GL_ARRAY_BUFFER, vboCone);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void GLScene::drawConeGeometry()
{
    glBindVertexArray(vaoCone);
    int slices = 20;
    int sideVertices = (slices + 1) * 2;

    glDrawArrays(GL_TRIANGLE_FAN, 0, sideVertices);
    glDrawArrays(GL_TRIANGLE_FAN, sideVertices, slices + 2);

    glBindVertexArray(0);
}

GLShape GLScene::setupCustomGeometry(const std::string &filename)
{
    GLShape shape(Custom);

    struct Vec3
    {
        float x, y, z;
    };
    struct Face
    {
        std::vector<int> vertices;
    };
    std::vector<float> vertices;

    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos)
    {
        return shape;
    }

    std::string extension = filename.substr(dotPos + 1);

    if (extension == "txt")
    {
        std::ifstream inputFile(filename);
        std::string line;

        if (inputFile.is_open())
        {
            while (std::getline(inputFile, line))
            {
                size_t first = line.find_first_not_of(" \t\n\r");
                if (first == std::string::npos)
                    continue;
                size_t last = line.find_last_not_of(" \t\n\r");
                line = line.substr(first, last - first + 1);

                std::istringstream iss(line);
                float x, y, z;
                if (iss >> x >> y >> z)
                {
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                }
            }
            inputFile.close();
        }
    }
    else if (extension == "stl")
    {
        struct Triangle
        {
            float vertices[3][3];
        };
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            return shape;
        }

        char header[5] = {0};
        file.read(header, 5);
        file.seekg(0, std::ios::beg);
        bool isASCII = std::strncmp(header, "solid", 5) == 0;
        std::vector<Triangle> triangles;

        if (isASCII)
        {
            std::string line;
            Triangle tri;
            int vertexIndex = 0;
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string word;
                ss >> word;
                if (word == "vertex")
                {
                    ss >> tri.vertices[vertexIndex][0] >> tri.vertices[vertexIndex][1] >> tri.vertices[vertexIndex][2];
                    vertexIndex++;
                    if (vertexIndex == 3)
                    {
                        triangles.push_back(tri);
                        vertexIndex = 0;
                    }
                }
            }
        }
        else
        {
            char binHeader[80];
            file.read(binHeader, 80);
            uint32_t numTriangles;
            file.read(reinterpret_cast<char *>(&numTriangles), sizeof(numTriangles));
            triangles.reserve(numTriangles);

            for (uint32_t i = 0; i < numTriangles; ++i)
            {
                Triangle tri;
                float normal[3];
                file.read(reinterpret_cast<char *>(normal), sizeof(normal));
                for (int v = 0; v < 3; ++v)
                {
                    file.read(reinterpret_cast<char *>(&tri.vertices[v][0]), sizeof(float) * 3);
                }
                uint16_t attribute;
                file.read(reinterpret_cast<char *>(&attribute), sizeof(attribute));
                triangles.push_back(tri);
            }
        }

        file.close();

        for (const auto &tri : triangles)
        {
            for (int v = 0; v < 3; ++v)
            {
                vertices.push_back(tri.vertices[v][0]);
                vertices.push_back(tri.vertices[v][1]);
                vertices.push_back(tri.vertices[v][2]);
            }
        }
    }
    else if (extension == "obj")
    {
        std::ifstream objStream(filename);
        if (!objStream.is_open())
        {
            std::cerr << "Failed to open OBJ file: " << filename << std::endl;
            return shape;
        }

        std::vector<Vec3> vertexList;
        std::vector<Face> faceList;
        std::string line;

        while (std::getline(objStream, line))
        {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v")
            {
                Vec3 v;
                ss >> v.x >> v.y >> v.z;
                vertexList.push_back(v);
            }
            else if (prefix == "f")
            {
                Face face;
                std::string vtn;
                while (ss >> vtn)
                {
                    size_t slashPos = vtn.find('/');
                    int vIndex = (slashPos != std::string::npos) ? std::stoi(vtn.substr(0, slashPos)) - 1 : std::stoi(vtn) - 1;
                    face.vertices.push_back(vIndex);
                }
                faceList.push_back(face);
            }
        }

        objStream.close();

        for (const auto &face : faceList)
        {
            if (face.vertices.size() < 3)
                continue;
            for (size_t i = 1; i + 1 < face.vertices.size(); ++i)
            {
                Vec3 v0 = vertexList[face.vertices[0]];
                Vec3 v1 = vertexList[face.vertices[i]];
                Vec3 v2 = vertexList[face.vertices[i + 1]];

                vertices.insert(vertices.end(), {v0.x, v0.y, v0.z,
                                                 v1.x, v1.y, v1.z,
                                                 v2.x, v2.y, v2.z});
            }
        }
    }
    else
    {
        std::cerr << "Unsupported file extension: " << extension << std::endl;
        return shape;
    }
    shape.vertexCount = vertices.size() / 3;
    glGenVertexArrays(1, &shape.vao);
    glGenBuffers(1, &shape.vbo);
    glBindVertexArray(shape.vao);
    glBindBuffer(GL_ARRAY_BUFFER, shape.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return shape;
}

// void GLScene::drawCustomGeometry(GLShape& shape) {
//     glBindVertexArray(shape.vao);
//     glDrawArrays(GL_TRIANGLES, 0, shape.vertexCount);
//     glBindVertexArray(0);
// }
