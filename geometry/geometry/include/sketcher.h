#ifndef SKETCHER_H
#define SKETCHER_H

#include "geometry.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

class Sketcher
{
private:
    std::vector<std::unique_ptr<Shape>> shapes;
    int nextID = 1;

public:
    void addShape();
    void deleteShape();
    void translateShape();
    void scaleShape();
    void rotateShape();
    void draw();
    void importSTL(); 
    void importOBJ();
    int getNextID() { return nextID; }
    void incrementID() { nextID++; }
};

#endif
