
#include <QApplication>

#include "gltkApplicationQt.hpp"
#include "gltkApplicationWindow.hpp"

int main(int argc, char *argv[])
{
  //Qt main application (handles exceptions)
  gltkApplicationQt app(argc, argv);

  //The main window
  gltkApplicationWindow appWindow;
  appWindow.show();

  //Start Qt Application
  return app.exec();
}
