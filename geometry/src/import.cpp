
#include "import.h"
#include <cstring>
#include <chrono>

void Import::listFiles(const std::string &directory, const std::string &extension)
{
    std::cout << "Available " << extension << " files:\n";
    for (const auto &entry : fs::directory_iterator(directory))
    {
        if (entry.path().extension() == extension)
        {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}

void Import::convertOBJtoDAT(const std::string &objFile, const std::string &datFile)
{

    auto startTime = std::chrono::high_resolution_clock::now();

    std::ifstream objStream(objFile);
    if (!objStream.is_open())
    {
        std::cerr << "Failed to open file: " << objFile << std::endl;
        return;
    }

    std::vector<Vec3> vertices;
    std::vector<Face> faces;
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
            vertices.push_back(v);
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
            faces.push_back(face);
        }
    }
    objStream.close();

    std::ofstream txtFile(datFile);
    if (!txtFile.is_open())
    {
        std::cerr << "Error creating txt file!" << std::endl;
        return;
    }

    for (const auto &face : faces)
    {
        if (face.vertices.size() < 3)
        {
            std::cerr << "Invalid face with less than 3 vertices, skipping." << std::endl;
            continue;
        }
        for (size_t i = 1; i + 1 < face.vertices.size(); ++i)
        {
            txtFile << vertices[face.vertices[0]].x << " " << vertices[face.vertices[0]].y << " " << vertices[face.vertices[0]].z << "\n";
            txtFile << vertices[face.vertices[i]].x << " " << vertices[face.vertices[i]].y << " " << vertices[face.vertices[i]].z << "\n";
            txtFile << vertices[face.vertices[i + 1]].x << " " << vertices[face.vertices[i + 1]].y << " " << vertices[face.vertices[i + 1]].z << "\n\n";
        }
    }
    txtFile.close();
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;
    std::cout << "================= OBJ File Statistics =================\n";
    std::cout << "Time taken to read file     : " << elapsedTime.count() << " seconds\n";
    
    std::cout << "Conversion completed! Data saved in " << datFile << std::endl;
}

std::string Import::generateOutputFileName(const std::string &outputFolder, int id)
{
    return outputFolder + "/shape_" + std::to_string(id) + ".txt";
}
void Import::convertSTLtoDAT(const std::string &stlFile, const std::string &datFile)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    std::ifstream inFile(stlFile, std::ios::binary);
    if (!inFile)
    {
        std::cerr << "Error: Cannot open STL file: " << stlFile << std::endl;
        return;
    }

    char header[5] = {0};
    inFile.read(header, 5);
    inFile.seekg(0, std::ios::beg);

    std::vector<Triangle> triangles;
    size_t memoryUsed = 0;
    uint32_t numTriangles = 0;
    uint32_t totalPoints = 0;

    if (std::strncmp(header, "solid", 5) == 0) // ASCII STL
    {
        std::string line;
        Triangle tri;
        int vertexIndex = 0;
        while (std::getline(inFile, line))
        {
            std::stringstream ss(line);
            std::string word;
            ss >> word;
            if (word == "vertex")
            {
                ss >> tri.vertices[vertexIndex][0] >> tri.vertices[vertexIndex][1] >> tri.vertices[vertexIndex][2];
                vertexIndex++;
                totalPoints++;
                if (vertexIndex == 3)
                {
                    triangles.push_back(tri);
                    memoryUsed += sizeof(Triangle);
                    numTriangles++;
                    vertexIndex = 0;
                }
            }
        }
    }
    else // Binary STL
    {
        char header[80];
        inFile.read(header, 80);
        inFile.read(reinterpret_cast<char *>(&numTriangles), sizeof(numTriangles));
        triangles.reserve(numTriangles);
        totalPoints = numTriangles * 3;

        for (uint32_t i = 0; i < numTriangles; ++i)
        {
            Triangle tri;
            float normal[3];
            inFile.read(reinterpret_cast<char *>(normal), sizeof(normal));
            for (int v = 0; v < 3; ++v)
            {
                inFile.read(reinterpret_cast<char *>(&tri.vertices[v][0]), sizeof(float) * 3);
            }
            char attribute[2];
            inFile.read(attribute, 2);
            triangles.push_back(tri);
            memoryUsed += sizeof(Triangle);
        }
    }
    inFile.close();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;

    // Print statistics
    std::cout << "================= STL File Statistics =================\n";
    std::cout << "Time taken to read file     : " << elapsedTime.count() << " seconds\n";
    std::cout << "Memory used to store data   : " << memoryUsed / 1024.0 << " KB\n";
    std::cout << "Number of triangles stored  : " << numTriangles << "\n";
    std::cout << "Number of points stored     : " << totalPoints << "\n";
    std::cout << "======================================================\n";

    // Write data to .dat file
    std::ofstream outFile(datFile);
    if (!outFile)
    {
        std::cerr << "Error creating txt file!" << std::endl;
        return;
    }

    for (const auto &tri : triangles)
    {
        for (int v = 0; v < 3; ++v)
        {
            outFile << tri.vertices[v][0] << " " << tri.vertices[v][1] << " " << tri.vertices[v][2] << "\n";
        }
        outFile << "\n\n";
    }
    outFile.close();
    std::cout << "Conversion completed! Data saved in " << datFile << std::endl;
}