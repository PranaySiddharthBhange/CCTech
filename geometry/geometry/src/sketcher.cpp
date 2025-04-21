#include "geometry.h"
#include <filesystem>
#include <algorithm>
namespace fs = std::filesystem;
void Sketcher::addShape()
{
    // Lambda to create, input, assign unique id, save, and store the shape.
    auto saveShape = [&](auto &&shape, const std::string &type)
    {
        std::cout << "Creating shape: " << type << std::endl;
        shape->input(); // Populate shape data

        // Assign unique ID using setter
        shape->setId(nextID);
        std::string filename = "./geometry/data/shape_" + std::to_string(nextID) + ".txt";
        nextID++; // Increment unique ID counter

        shape->save(filename);              // Save shape to file
        shapes.push_back(std::move(shape)); // Store shape in vector
        std::cout << "Shape " << type << " saved with filename: " << filename << std::endl;
    };

    int choice = displayMenu();
    switch (choice)
    {
    case 1:
        saveShape(std::make_unique<Point2D>(), "Point2D");
        break;
    case 2:
        saveShape(std::make_unique<Point3D>(), "Point3D");
        break;
    case 3:
        saveShape(std::make_unique<Line2D>(), "Line2D");
        break;
    case 4:
        saveShape(std::make_unique<Line3D>(), "Line3D");
        break;
    case 5:
        saveShape(std::make_unique<Rectangle>(), "Rectangle");
        break;
    case 6:
        saveShape(std::make_unique<Circle>(), "Circle");
        break;
    case 7:
        saveShape(std::make_unique<Cuboid>(), "Cuboid");
        break;
    case 8:
        saveShape(std::make_unique<Cuboid>(), "Cuboid (Duplicate choice)");
        break;
    case 9:
        saveShape(std::make_unique<Polyline2D>(), "Polyline2D");
        break;
    case 10:
        saveShape(std::make_unique<Polyline3D>(), "Polyline3D");
        break;
    case 11:
        saveShape(std::make_unique<Bezier2D>(), "Bezier2D");
        break;
    case 12:
        saveShape(std::make_unique<Bezier3D>(), "Bezier3D");
        break;
    case 13:
        saveShape(std::make_unique<Polygon>(), "Polygon");
        break;
    case 14:
        saveShape(std::make_unique<Polygon>(), "Polygon (Duplicate choice)");
        break;
    case 15:
        saveShape(std::make_unique<Sphere>(), "Sphere");
        break;
    case 16:
        saveShape(std::make_unique<Cylinder>(), "Cylinder");
        break;
    default:
        std::cout << "Invalid choice!" << std::endl;
        break;
    }
}

void Sketcher::deleteShape()
{
    if (shapes.empty())
    {
        std::cout << "No shapes to delete!" << std::endl;
        return;
    }

    // List available shapes by their unique IDs.
    std::cout << "Available shapes:" << std::endl;
    for (const auto &shape : shapes)
    {
        std::cout << "Shape ID: " << shape->getId() << std::endl; // Use getId()
    }

    std::cout << "Enter the unique shape ID to delete: ";
    int idToDelete;
    std::cin >> idToDelete;

    // Find the shape by its unique ID.
    auto it = std::find_if(shapes.begin(), shapes.end(),
                           [idToDelete](const std::unique_ptr<Shape> &shape)
                           {
                               return shape->getId() == idToDelete; // Use getId()
                           });

    if (it == shapes.end())
    {
        std::cout << "Shape with ID " << idToDelete << " not found!" << std::endl;
        return;
    }

    // Build the filename from the unique ID.
    std::string filename = "./geometry/data/shape_" + std::to_string(idToDelete) + ".txt";

    // Delete the file if it exists.
    if (fs::exists(filename))
    {
        if (fs::remove(filename))
        {
            std::cout << "Deleted file: " << filename << std::endl;
        }
        else
        {
            std::cerr << "Failed to delete file: " << filename << std::endl;
        }
    }
    else
    {
        std::cout << "File not found: " << filename << std::endl;
    }

    // Remove the shape from the vector.
    shapes.erase(it);
    std::cout << "Shape deleted successfully!" << std::endl;
}

void Sketcher::rotateShape()
{
    if (shapes.empty())
    {
        std::cout << "No shapes to rotate!" << std::endl;
        return;
    }

    // List available shapes by their unique IDs.
    std::cout << "Available shapes:" << std::endl;
    for (const auto &shape : shapes)
    {
        std::cout << "Shape ID: " << shape->getId() << std::endl; // Use getId()
    }

    std::cout << "Enter the unique shape ID to rotate: ";
    int id;
    std::cin >> id;

    // Find the shape by its unique ID.
    auto it = std::find_if(shapes.begin(), shapes.end(), [id](const std::unique_ptr<Shape> &s)
                           {
                               return s->getId() == id; // Use getId()
                           });

    if (it == shapes.end())
    {
        std::cout << "Shape with ID " << id << " not found." << std::endl;
        return;
    }

    std::string filename = "./geometry/data/shape_" + std::to_string(id) + ".txt";
    std::cout << "Attempting to rotate shape and save to file: " << filename << std::endl;

    (*it)->rotate(filename); // Rotate and save the shape

    std::cout << "Shape rotated and saved successfully!" << std::endl;
}

void Sketcher::translateShape()
{
    if (shapes.empty())
    {
        std::cout << "No shapes to translate!" << std::endl;
        return;
    }

    // List available shapes by their unique IDs.
    std::cout << "Available shapes:" << std::endl;
    for (const auto &shape : shapes)
    {
        std::cout << "Shape ID: " << shape->getId() << std::endl; // Use getId()
    }

    std::cout << "Enter the unique shape ID to translate: ";
    int id;
    std::cin >> id;

    // Find the shape by its unique ID.
    auto it = std::find_if(shapes.begin(), shapes.end(), [id](const std::unique_ptr<Shape> &s)
                           {
                               return s->getId() == id; // Use getId()
                           });

    if (it == shapes.end())
    {
        std::cout << "Shape with ID " << id << " not found." << std::endl;
        return;
    }

    // Get translation vector (dx, dy, dz)
    double dx, dy, dz;
    std::cout << "Enter translation values (dx, dy, dz): ";
    std::cin >> dx >> dy >> dz;

    std::string filename = "./geometry/data/shape_" + std::to_string(id) + ".txt";
    std::cout << "Attempting to translate shape and save to file: " << filename << std::endl;

    (*it)->translate(dx, dy, dz);
    (*it)->save(filename);

    std::cout << "Shape translated and saved successfully!" << std::endl;
}

void Sketcher::scaleShape()
{
    if (shapes.empty())
    {
        std::cout << "No shapes to scale!" << std::endl;
        return;
    }

    // List available shapes by their unique IDs.
    std::cout << "Available shapes:" << std::endl;
    for (const auto &shape : shapes)
    {
        std::cout << "Shape ID: " << shape->getId() << std::endl; // Use getId()
    }

    std::cout << "Enter the unique shape ID to scale: ";
    int id;
    std::cin >> id;

    // Find the shape by its unique ID.
    auto it = std::find_if(shapes.begin(), shapes.end(), [id](const std::unique_ptr<Shape> &s)
                           {
                               return s->getId() == id; // Use getId()
                           });

    if (it == shapes.end())
    {
        std::cout << "Shape with ID " << id << " not found." << std::endl;
        return;
    }

    double factorX, factorY, factorZ;
    std::cout << "Enter scale factors (factorX, factorY, factorZ): ";
    std::cin >> factorX >> factorY >> factorZ;

    std::string filename = "./geometry/data/shape_" + std::to_string(id) + ".txt";
    std::cout << "Attempting to scale shape and save to file: " << filename << std::endl;

    (*it)->scale(factorX, factorY, factorZ);
    (*it)->save(filename);

    std::cout << "Shape scaled and saved successfully!" << std::endl;
}

void Sketcher::draw()
{
    // Prepare the Gnuplot command
    std::stringstream plotCommand;
    plotCommand << "splot ";

    // Directory path containing the shape data files
    std::string dirPath = "./geometry/data/";

    // Vector to store found file names
    std::vector<std::string> shapeFiles;

    // Iterate through files in the directory
    for (const auto &entry : fs::directory_iterator(dirPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            shapeFiles.push_back(entry.path().string());
            std::cout << "Found shape file: " << entry.path().string() << std::endl;
        }
    }

    // Check if there are files to plot
    if (shapeFiles.empty())
    {
        std::cerr << "No shape files found in directory: " << dirPath << std::endl;
        return;
    }

    // Append each file to the plot command
    for (size_t i = 0; i < shapeFiles.size(); ++i)
    {
        if (i > 0)
            plotCommand << ", "; // Add a comma between the files
        plotCommand << "'" << shapeFiles[i] << "' with lines title 'Shape " << i + 1 << "'";
    }

    // Save the final Gnuplot command as a string
    std::string finalCommand = plotCommand.str();

    // Print the final command (for debugging)
    std::cout << "Final Gnuplot Command: " << finalCommand << std::endl;

    // Use Gnuplot to plot all shapes at once
    system(("gnuplot -e \"" + finalCommand + "; pause -1\"").c_str());
}

void Sketcher::importSTL()
{
    std::string stlDirectory = "./geometry/assets/stl";
    std::string outputDirectory = "./geometry/data";

    // List available STL files
    Import::listFiles(stlDirectory,".stl");

    std::string selectedFile;
    std::cout << "Enter STL filename to import: ";
    std::cin >> selectedFile;

    std::string stlFilePath = stlDirectory + "/" + selectedFile;
    std::string datFilePath = Import::generateOutputFileName(outputDirectory,getNextID());

    // Convert STL to DAT format
    Import::convertSTLtoDAT(stlFilePath, datFilePath);

    std::cout << "STL file imported and saved as: " << datFilePath << std::endl;
    incrementID();
}
void Sketcher::importOBJ()
{
    std::string stlDirectory = "./geometry/assets/obj";
    std::string outputDirectory = "./geometry/data";

    // List available STL files
    Import::listFiles(stlDirectory,".obj");

    std::string selectedFile;
    std::cout << "Enter OBJ filename to import: ";
    std::cin >> selectedFile;

    std::string stlFilePath = stlDirectory + "/" + selectedFile;
    std::string datFilePath = Import::generateOutputFileName(outputDirectory,getNextID());

    // Convert OBJ to DAT format
    Import::convertOBJtoDAT(stlFilePath, datFilePath);

    std::cout << "OBJ file imported and saved as: " << datFilePath << std::endl;
    incrementID();
}




