#include <QApplication> 
#include "mainwindow.h" 

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(1000, 600);
    window.setWindowTitle("Sketcher");
    window.show();
    return app.exec();
}