It connects the user interface (buttons, controls, dropdowns) with the OpenGL rendering logic (`GLScene`).

---

## Features & Responsibilities

| Feature | What It Does |
|--------|---------------|
| Central OpenGL Area (`GLScene`) | Renders all 3D shapes and responds to transformations |
| Shape Dropdown (`QComboBox`) | Lets you select which shape to add (Cube, Sphere, Cone, Cylinder, Custom) |
| Add Shape Button | Adds the selected shape to the OpenGL scene |
| Shape List (`QListWidget`) | Displays all added shapes so the user can select one |
| Transformation Controls (`QDoubleSpinBox`) | Allow changing rotation, position, and scale of the selected shape |
| Delete Shape Button | Removes the selected shape from the list and 3D view |

---

## Core Components

### Constructor: `MainWindow::MainWindow(QWidget* parent)`
Initializes and lays out all UI elements:
- Sets `GLScene` as the central widget.
- Creates a **dockable control panel** on the right with all user input tools.

---

## User Interaction Methods (Slots)

### `void onAddShape()`
**What it does:**
- Adds a new shape to the 3D scene.
- If **Custom** is selected, opens a file dialog and loads `.obj`, `.stl`, or `.txt` files.
- Updates the shape list on the right.

---

### `void onShapeSelected(int row)`
**What it does:**
- When the user clicks a shape in the list, this method:
  - Loads its transformation data (rotation, translation, scale)
  - Updates the UI controls to reflect the selected shape’s state

---

### `void onTransformChanged()`
**What it does:**
- When the user changes spin box values (rotation, position, scale):
  - Applies the new values directly to the selected shape in `GLScene`
  - Triggers a redraw with updated values

---

### `void onDeleteShape()`
**What it does:**
- Removes the selected shape from:
  - The shape list (`QListWidget`)
  - The OpenGL scene (`GLScene::shapes`)

---

##  UI Breakdown

| UI Component | Purpose |
|--------------|---------|
| `shapeComboBox` | Choose the type of shape to add |
| `addShapeButton` | Adds the chosen shape to the scene |
| `shapeListWidget` | Lets the user select and manage shapes |
| `rotationXSpin`, `rotationYSpin`, `rotationZSpin` | Rotate shape along X, Y, Z axes |
| `translateXSpin`, `translateYSpin`, `translateZSpin` | Move shape in 3D space |
| `scaleSpin` | Uniformly scale the shape |
| `deleteShapeButton` | Deletes selected shape |

---
