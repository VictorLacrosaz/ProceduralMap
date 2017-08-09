#pragma once

#ifndef GLTK_APPLICATION_WIGDET_HPP
#define GLTK_APPLICATION_WIGDET_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include <QtOpenGL/QGLWidget>
#include <QMouseEvent>

#include "AxisHelper.hpp"
#include "gltkRenderInteractor.hpp"
#include "mesh.hpp"
#include "mesh_opengl.hpp"

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


protected:

  //-------------------------------------------------------
  //  GL callbacks
  //-------------------------------------------------------
  /** Setup the OpenGL rendering mode */
  void initializeGL();
  /** Rendering callback */
  void paintGL();
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
};

#endif
