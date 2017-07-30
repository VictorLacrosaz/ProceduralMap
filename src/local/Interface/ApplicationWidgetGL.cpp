#include "ApplicationWidgetGL.hpp"
#include "Project_Config.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "error_handling.hpp"
#include "glutils.hpp"
#include "mat4.hpp"
#include "mat3.hpp"

#include <cmath>
#include <iostream>
#include <unistd.h>


ApplicationWidgetGL::ApplicationWidgetGL(const QGLFormat& format,QGLWidget *parent) :
    QGLWidget(format,parent), DrawState(true)
{
    QWidget::setFocusPolicy(Qt::StrongFocus);
    startTimer(25); //start timer every 25ms
    renderInteractor = RenderInteractor();
}

ApplicationWidgetGL::~ApplicationWidgetGL()
{}

//-------------------------------------------------------
//  Rendering callbacks
//-------------------------------------------------------
void ApplicationWidgetGL::paintGL()
{
  //clear screen
  int width = renderInteractor.GetWindowSize()[0];
  int height = renderInteractor.GetWindowSize()[1];
  glViewport (0, 0, width, height);                    PRINT_OPENGL_ERROR();
  glClearColor (1.0f, 1.0f, 1.0f, 1.0f);               PRINT_OPENGL_ERROR();
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();

  //draw 3D scene
  if (DrawState)
  {
    renderInteractor.Render();
  }
}

//-------------------------------------------------------
//  Interaction callbacks
//-------------------------------------------------------
void ApplicationWidgetGL::keyPressEvent(QKeyEvent *event)
{
    QString k=event->text();

    int current=event->key();
    Qt::KeyboardModifiers mod=event->modifiers();

    // We can quit the scene with 'Q'
    if( (mod&Qt::ShiftModifier)!=0 && (current==Qt::Key_Q) )
    {
        std::cout<<"\n[EXIT OK]\n\n"<<std::endl;
        this->window()->close();
    }

    QGLWidget::keyPressEvent(event);
}


void ApplicationWidgetGL::mousePressEvent(QMouseEvent *event)
{
  renderInteractor.SetLastEventPosition(event->x(),event->y());
//  updateGL(); PRINT_OPENGL_ERROR();
}


void ApplicationWidgetGL::mouseMoveEvent(QMouseEvent *event)
{
  //Update cursor position
  renderInteractor.SetEventPosition(event->x(), event->y());

  int const ctrl_pressed  = (event->modifiers() & Qt::ControlModifier);
  int const shift_pressed = (event->modifiers() & Qt::ShiftModifier);

  // Left button controls the rotation
  if (!ctrl_pressed && !shift_pressed && (event->buttons() & Qt::LeftButton) )
  {
    renderInteractor.TrackBallRotate();
  }

  // Right button controls the zoom
  if (!ctrl_pressed && !shift_pressed && (event->buttons() & Qt::RightButton))
  {
    renderInteractor.TrackBallZoom();
  }

  // Shift+Left button controls the window translation (left/right, bottom/up)
  if (!ctrl_pressed && shift_pressed && (event->buttons() & Qt::LeftButton))
  {
    renderInteractor.MoveForward();
    renderInteractor.MoveRight();
  }

  // Shift+Right button enables to translate forward/backward
  if (!ctrl_pressed && shift_pressed && (event->buttons() & Qt::RightButton))
  {
    renderInteractor.MoveUp();
  }

  //Store last cursor position
  renderInteractor.SetLastEventPosition(event->x(),event->y());
}


void ApplicationWidgetGL::timerEvent(QTimerEvent *event)
{
    event->accept();
    updateGL(); PRINT_OPENGL_ERROR();
}
//-------------------------------------------------------

//-------------------------------------------------------
//  GL callbacks
//-------------------------------------------------------
void ApplicationWidgetGL::initializeGL()
{
    //Init OpenGL
    InitGL();

    //Init Scene 3D
    renderInteractor.Initialize();
    GLuint texture = LoadTextureFromFile("white.jpg");
    renderInteractor.GetRenderManager().SetTextureDefault(texture);

    //Activate depth buffer
    glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
}

void ApplicationWidgetGL::resizeGL(int const width,int const height)
{
    renderInteractor.SetWindowSize( width, height );
    glViewport(0,0, width, height); PRINT_OPENGL_ERROR();
}
//-------------------------------------------------------

//-------------------------------------------------------
//  Initialize OpenGL Context
//-------------------------------------------------------
void ApplicationWidgetGL::InitGL()
{
  //Print Context
  PrintGLInfo();
  PrintGLContext();

  InitGlew();
}

void ApplicationWidgetGL::InitGlew()
{
  //Initialize Glew
  GLenum GlewInitResult = glewInit();

  //Error handling
  if (GlewInitResult != GLEW_OK)
  {
    std::cerr << "Error: " << glewGetErrorString(GlewInitResult)
      << std::endl;
    exit(EXIT_FAILURE);
  }

  //Print info
  std::cout << "Glew initialized (" << glewGetString(GLEW_VERSION) << ")"
    << std::endl;
}

void ApplicationWidgetGL::PrintGLContext() const
{
  std::cout << "Current OpenGL context: " <<
    context()->format().majorVersion() << "." <<
    context()->format().minorVersion() << std::endl;
}
//-------------------------------------------------------

//-------------------------------------------------------
//  UI actions binding
//-------------------------------------------------------
void ApplicationWidgetGL::ToggleDrawState()
{
    DrawState =! DrawState;
    updateGL();
}

void ApplicationWidgetGL::SetWireframe(bool const wireframe)
{
  if (wireframe)
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);    PRINT_OPENGL_ERROR();
  }
  else
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);    PRINT_OPENGL_ERROR();
  }

  updateGL();
}
//-------------------------------------------------------

GLuint ApplicationWidgetGL::LoadTextureFromFile(std::string const& filename)
{
    std::string data_dir = DATA_DIR;
    data_dir.append( "/" );

    if(access((data_dir + filename).data(),F_OK) == -1)
    {
        std::cerr<<"Cannot load texture file "<<(data_dir + filename)<<std::endl;
        std::cerr<<"Wrong path ?"<<std::endl;
        abort();
    }

    GLuint const value=this->bindTexture(QString((data_dir +filename).data()), GL_TEXTURE_2D);

    if(value==0)
        throw cpe::exception_cpe("Failed to load texture "+(data_dir + filename),EXCEPTION_PARAMETERS_CPE);

    return value;
}
