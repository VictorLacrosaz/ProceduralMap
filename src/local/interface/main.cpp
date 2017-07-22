
#include <QApplication>

#include "application_qt.hpp"
#include "myWindow.hpp"

int main(int argc, char *argv[])
{
    //Qt main application (handles exceptions)
    application_qt app(argc, argv);

    //The main window
    myWindow win;
    win.show();

    //Start Qt Application
    return app.exec();
}
