### `class MainWindow : public QMainWindow`
This is the main GUI window that connects UI controls to the 3D OpenGL scene.

---

### UI Components
| Variable | Purpose |
|----------|---------|
| `glScene` | OpenGL scene renderer (from `GLScene`) |
| `shapeComboBox` | Dropdown to choose shape type (Cube, Sphere, etc.) |
| `shapeListWidget` | Shows list of added shapes |
| `rotationX/Y/ZSpin` | Spin boxes to rotate selected shape |
| `translateX/Y/ZSpin` | Move shape along 3D axes |
| `scaleSpin` | Uniform scale control |

---

### Private Slots (Signal Handlers)
| Method | Purpose |
|--------|---------|
| `onAddShape()` | Adds new shape using selected type |
| `onShapeSelected(int row)` | Selects shape from the list |
| `onTransformChanged()` | Updates shape's rotation/translation/scale |
| `onDeleteShape()` | Removes selected shape from scene |

---
