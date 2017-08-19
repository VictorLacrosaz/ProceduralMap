#include "gltkApplicationWindow.hpp"

#include "gltkApplicationWidgetGL.hpp"
#include "error_handling.hpp"
#include "ui_gltkApplicationWindow.h"

#include <iostream>

gltkApplicationWindow::gltkApplicationWindow(QWidget *parent)
  :QMainWindow(parent),ui(new Ui::MainWindow)
{
  try
  {
    //Setup window layout
    ui->setupUi(this);

    //Create openGL context
    QGLFormat qglFormat;
    qglFormat.setVersion(1,2);

    //Create OpenGL Widget renderer
    glWidget=new gltkApplicationWidgetGL(qglFormat);

    //Add the OpenGL Widget into the layout
    ui->layout_scene->addWidget(glWidget);
  }
  catch(cpe::exception_cpe const& e)
  {
    std::cout<<std::endl<<e.report_exception()<<std::endl;
  }

  //Connect slot and signals
  connect(ui->quit,SIGNAL(clicked()),this,SLOT(action_quit()));
  connect(ui->draw,SIGNAL(clicked()),this,SLOT(action_draw()));
  connect(ui->wireframe,SIGNAL(clicked()),this,SLOT(action_wireframe()));

}

gltkApplicationWindow::~gltkApplicationWindow()
{}

void gltkApplicationWindow::action_quit()
{
  close();
}

void gltkApplicationWindow::action_draw()
{
  glWidget->ToggleDrawState();
}

void gltkApplicationWindow::action_wireframe()
{
  bool const wireframeState = ui->wireframe->isChecked();
  glWidget->SetWireframe(wireframeState);
}

