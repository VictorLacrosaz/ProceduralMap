#pragma once

#ifndef GLTK_APPLICATION_WINDOW_HPP
#define GLTK_APPLICATION_WINDOW_HPP


#include <QMainWindow>

//forward declaration
namespace Ui
{
class MainWindow;
}
class gltkApplicationWidgetGL;


/** Declaration of the Window class */
class gltkApplicationWindow: public QMainWindow
{
  Q_OBJECT

public:

    gltkApplicationWindow(QWidget *parent=NULL);
    ~gltkApplicationWindow();

private slots:

    /** Quit the application */
    void action_quit();
    /** Enable the drawing of the meshes */
    void action_draw();
    /** Set the Wireframe mode for the meshes */
    void action_wireframe();

private:

    /** Layout for the Window */
    Ui::MainWindow *ui;
    /** The OpenGL Widget */
    gltkApplicationWidgetGL *glWidget;
};

#endif
