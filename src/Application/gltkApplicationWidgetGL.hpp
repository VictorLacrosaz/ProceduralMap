#pragma once

#ifndef GLTK_APPLICATION_WIGDET_HPP
#define GLTK_APPLICATION_WIGDET_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include <QtOpenGL/QGLWidget>
#include <QMouseEvent>
#include <QTime>

#include "AxisHelper.hpp"
#include "gltkRenderInteractor.hpp"

#include <ui_gltkApplicationWidgetGL.h>


namespace Ui
{
 class GLWindow;
}


/** Qt Widget to render OpenGL scene */
class gltkApplicationWidgetGL : public QGLWidget
{
  Q_OBJECT

public:

  gltkApplicationWidgetGL(const QGLFormat& format, QGLWidget *parent = 0);
  ~gltkApplicationWidgetGL();

  //-------------------------------------------------------
  //  UI action binding
  //-------------------------------------------------------
  /** \brief Toggle drawing of the scene */
  void ToggleDrawState();
  /** \brief Set wireframe representation mode on/off */
  void SetWireframe(bool const wireframe);

  /** \brief Load a texture given by its filename */
  GLuint LoadTextureFromFile(std::string const& filename);

private slots:

  /** \brief Toggle drawing of the scene */
  void Draw();



protected:

  //-------------------------------------------------------
  //  GL callbacks
  //-------------------------------------------------------
  /** Setup the OpenGL rendering mode */
  void initializeGL();
  /** Rendering callbacks */
  void paintGL();
  void paintEvent(QPaintEvent* event);
  /** Function called when the window is resized */
  void resizeGL(int width, int height);

  //-------------------------------------------------------
  //  Interaction callbacks
  //-------------------------------------------------------
  /** Function called a button of the mouse is pressed */
  void mousePressEvent(QMouseEvent *event);
  /** Function called when the mouse is moved */
  void mouseMoveEvent(QMouseEvent *event);
  /** Function called when keyboard is pressed */
  void keyPressEvent(QKeyEvent *event);
  /** Function called when mouse wheel is moved */
  void wheelEvent(QWheelEvent *event);

  /** Function called on timer overflow */
  void timerEvent(QTimerEvent *event);

private:

  /** Init the OpenGL rendering mode once at the beginning */
  void InitGL();
  /** Init Glew once at the beginning. (called from InitGL()) */
  void InitGlew();
  /** Print on the command line the actual version of the OpenGL Context */
  void PrintGLContext() const;


  /** Toggle rendering of the 3D scene*/
  bool DrawState;

  /** Handle interactions with the rendering manager */
  gltkRenderInteractor RenderInteractor;


  /** FPS Calculation */
  int AimFPS;
  QTime TimeElapsed;
  int FPS;
  int NbFrame;
  int Limit;

  /** Layout for the Window */
  Ui::GLWindow* ui;



};

#endif
