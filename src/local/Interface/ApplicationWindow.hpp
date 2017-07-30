#pragma once

#ifndef MY_WINDOW_HPP
#define MY_WINDOW_HPP


#include <QMainWindow>


//forward declaration
namespace Ui
{
class MainWindow;
}
class ApplicationWidgetGL;

/** Declaration of the Window class */
class ApplicationWindow: public QMainWindow
{
  Q_OBJECT

public:

    ApplicationWindow(QWidget *parent=NULL);
    ~ApplicationWindow();

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
    ApplicationWidgetGL *glWidget;
};

#endif
