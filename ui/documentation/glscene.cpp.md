
## Key Components

### 1. **Shapes Data Structure**
   - `GLScene` maintains a list of shapes, each of which is a 3D object with properties such as position, rotation, and scale.
   - The shapes are rendered in a 3D space using OpenGL.

### 2. **OpenGL Context**
   - `GLScene` manages the OpenGL context used for rendering. It takes care of setting up shaders, buffers, and rendering loops.

### 3. **Transformation Functions**
   - Functions in `GLScene` allow applying rotations, translations, and scaling to shapes in the 3D scene.

---

## Key Methods and Functions

### 1. **`GLScene::addShape(ShapeType type)`**

   - **Purpose**: Adds a new shape of the specified type (Cube, Sphere, Cylinder) to the scene.
   - **Parameters**:
     - `ShapeType type`: The type of shape to add (Cube, Sphere, Cylinder).
   - **Description**:
     - Creates a new shape object based on the type and adds it to the list of shapes in the scene.
     - Initializes the shape with default position, rotation, and scale values.


---

### 2. **`GLScene::selectShape(int index)`**

   - **Purpose**: Selects a shape from the list of shapes.
   - **Parameters**:
     - `int index`: The index of the shape in the list.
   - **Description**:
     - Selects a shape from the internal list of shapes. Once selected, this shape can be transformed (rotated, translated, scaled).
     - The selected shape becomes the active shape for transformation.

---

### 3. **`GLScene::applyTransformation()`**

   - **Purpose**: Applies the current transformation (rotation, translation, scale) to the selected shape.
   - **Description**:
     - This method updates the selected shape's properties based on the current transformation values.
     - It applies rotation, translation, and scaling to the shape in real-time.
     - Typically called after the user changes transformation values (e.g., by using spin boxes for rotation and translation).

---

### 4. **`GLScene::deleteShape(int index)`**

   - **Purpose**: Deletes the shape at the specified index from the scene.
   - **Parameters**:
     - `int index`: The index of the shape to delete in the list.
   - **Description**:
     - Removes the shape at the given index from the list of shapes in the scene.
     - The shape is removed from the OpenGL context, and the scene is updated accordingly.

---

### 5. **`GLScene::renderScene()`**

   - **Purpose**: Renders the scene by drawing all the shapes in the list.
   - **Description**:
     - This function is called during the rendering loop to display all the shapes in the scene.
     - It iterates over the list of shapes and renders each shape with its current transformations (rotation, translation, scale).
---

## OpenGL Rendering Pipeline

`GLScene` sets up the OpenGL rendering pipeline to render 3D objects. This involves:

- **Shaders**: Vertex and fragment shaders are used to draw the shapes in the scene.
- **Buffers**: Vertex buffers and index buffers are used to store shape data for rendering.
- **Transformation Matrices**: Transformation matrices are used to apply rotation, translation, and scaling to shapes in the OpenGL space.

---
