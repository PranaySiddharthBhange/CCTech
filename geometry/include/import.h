
#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct Triangle
{
    float vertices[3][3];
};

struct Vec3
{
    float x, y, z;
};

struct Face
{
    std::vector<int> vertices;
};

class Import
{
public:
    static void listFiles(const std::string &directory, const std::string &extension);
    static void convertSTLtoDAT(const std::string &stlFile, const std::string &datFile);
    static void convertOBJtoDAT(const std::string &objFile, const std::string &datFile);
    static std::string generateOutputFileName(const std::string &outputFolder, int id);
};

#endif
