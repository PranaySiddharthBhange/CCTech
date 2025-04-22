### `struct GLShape`
Represents a 3D shape in the scene.

| Property | Purpose |
|----------|---------|
| `type` | Shape type: Cube, Sphere, Cylinder, Cone, or Custom |
| `rotationX/Y/Z` | Rotation angles (in degrees) around each axis |
| `translateX/Y/Z` | Position in 3D space |
| `scale` | Uniform scaling factor |
| `vao`, `vbo`, `vertexCount` | Used only for custom shapes |

---

### `class GLScene : public QOpenGLWidget`
The main OpenGL scene widget for rendering and interaction.

####  Public Methods
| Method | Purpose |
|--------|---------|
| `addShape(ShapeType)` | Add a new shape to the scene |
| `deleteShape(int)` | Remove shape at the given index |
| `getSelectedShape()` | Returns pointer to selected shape |
| `setupCustomGeometry(filename)` | Load custom `.obj`/`.stl` mesh |

####  Protected Overrides
| Method | Purpose |
|--------|---------|
| `initializeGL()` | Initialize shaders and geometry buffers |
| `resizeGL(w, h)` | Handle widget resizing |
| `paintGL()` | Render the 3D scene |
| `mousePressEvent()` | Handle mouse clicks (e.g., select shape or control camera) |
| `mouseMoveEvent()` | Rotate camera or transform shapes |
| `wheelEvent()` | Zoom in/out |

---

###  Private Drawing & Setup Methods
| Method | Purpose |
|--------|---------|
| `setupShaders()` | Compile and link GLSL shaders |
| `setupCubeGeometry()` | Upload cube vertex data |
| `drawCubeGeometry()` | Render cube |
| `setupSphereGeometry()` | Generate sphere data |
| `drawSphereGeometry()` | Render sphere |
| `setupCylinderGeometry()` / `drawCylinderGeometry()` | Same for cylinder |
| `setupConeGeometry()` / `drawConeGeometry()` | Same for cone |

---

### Camera Controls
| Variable | Purpose |
|----------|---------|
| `cameraDistance` | Zoom level |
| `cameraAzimuth` | Horizontal rotation angle |
| `cameraElevation` | Vertical angle |

---
