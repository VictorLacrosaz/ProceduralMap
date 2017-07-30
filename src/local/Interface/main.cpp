
#include <QApplication>

#include "ApplicationQt.hpp"
#include "ApplicationWindow.hpp"

int main(int argc, char *argv[])
{
  //Qt main application (handles exceptions)
  ApplicationQt app(argc, argv);

  //The main window
  ApplicationWindow appWindow;
  appWindow.show();

  //Start Qt Application
  return app.exec();
}
