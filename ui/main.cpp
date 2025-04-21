// main.cpp
#include <QApplication>  // Includes the QApplication class for managing application-wide resources
#include "mainwindow.h"  // Includes the MainWindow class definition

int main(int argc, char* argv[])
{
    // Create a QApplication object to manage application-wide settings and the event loop
    QApplication app(argc, argv);

    // Create an instance of MainWindow, which is the main UI window of the application
    MainWindow window;

    // Set the size of the main window to 1000x600 pixels
    window.resize(1000, 600);

    // Set the title of the main window to "Sketcher"
    window.setWindowTitle("Sketcher");

    // Display the main window on the screen
    window.show();

    // Start the application's event loop and return its exit status
    return app.exec();
}