#include "gltkApplicationWidgetGL.hpp"
#include "ProjectConfig.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include <cmath>
#include <iostream>
#include <unistd.h>

#include "error_handling.hpp"
#include "glUtils.hpp"
#include "mat4.hpp"
#include "mat3.hpp"


gltkApplicationWidgetGL::gltkApplicationWidgetGL(const QGLFormat& format,QGLWidget *parent) :
  QGLWidget(format,parent),DrawState(true),AimFPS(90),FPS(0),NbFrame(0),Limit(AimFPS/4),ui(new Ui::GLWindow)
{
  QWidget::setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);

  int TimerCallback = (1.0 / AimFPS) * 1000;
  startTimer(TimerCallback); //start timer every TimerCallBack overflow to reach AimFPS
  RenderInteractor = gltkRenderInteractor();

  //Setup UI for OpenGL window
  ui->setupUi(this);

}

gltkApplicationWidgetGL::~gltkApplicationWidgetGL()
{}

//-------------------------------------------------------
//  Rendering callbacks
//-------------------------------------------------------
void gltkApplicationWidgetGL::paintGL()
{
  //Activate depth buffer
  glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
  //clear screen
  int width = RenderInteractor.GetWindowSize()[0];
  int height = RenderInteractor.GetWindowSize()[1];
  glViewport (0, 0, width, height);                    PRINT_OPENGL_ERROR();
  glClearColor (1.0f, 1.0f, 1.0f, 1.0f);               PRINT_OPENGL_ERROR();
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();

  //draw 3D scene
  if (DrawState)
  {
    RenderInteractor.Render();
  }
   //Desactivate depth buffer
   glDisable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
}
void gltkApplicationWidgetGL::paintEvent(QPaintEvent* event)
{

  QPainter  painter(this);

  //FPS Calculation
  if(NbFrame > Limit)
  {
    FPS =  float(NbFrame) / (float(TimeElapsed.elapsed()) / 1000);
    NbFrame = 0;
    TimeElapsed.restart();

  }
  NbFrame++;


  /** *** OpenGL render *** **/
  painter.beginNativePainting();
  paintGL();
  painter.endNativePainting();
  /** ***               *** **/

  //Painter setup
  QPen myPen(Qt::red);
  painter.setPen(myPen);
  int width = RenderInteractor.GetWindowSize()[0];
  int height = RenderInteractor.GetWindowSize()[1];
  QFont font=painter.font() ;
  font.setPointSizeF(20);
  font.setWeight(QFont::Bold);
  font.setFamily("tahoma");
  painter.setFont(font);
  QString s = QString::number(FPS);

  painter.drawText(QRect(0,0,width/10,height/10),s);
  painter.end();

}

//-------------------------------------------------------
//  Interaction callbacks
//-------------------------------------------------------
void gltkApplicationWidgetGL::keyPressEvent(QKeyEvent *event)
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


void gltkApplicationWidgetGL::mousePressEvent(QMouseEvent *event)
{
  //Init last event position
  RenderInteractor.SetLastEventPosition(event->x(),event->y());
  //Update event position
  RenderInteractor.SetEventPosition(event->x(),event->y());

  //WIP : Ray picking debug
  int const ctrl_pressed  = (event->modifiers() & Qt::ControlModifier);
  if(ctrl_pressed)
  {
    RenderInteractor.Pick();
  }
}

void gltkApplicationWidgetGL::wheelEvent(QWheelEvent *event)
{
  int numDegrees = event->delta() / 8;
  int numSteps = numDegrees / 15;

  RenderInteractor.SetWheelMouv(numSteps);

  RenderInteractor.TrackBallZoomWheel();

}

void gltkApplicationWidgetGL::mouseMoveEvent(QMouseEvent *event)
{
  //Update cursor position
  RenderInteractor.SetEventPosition(event->x(), event->y());

  int const ctrl_pressed  = (event->modifiers() & Qt::ControlModifier);
  int const shift_pressed = (event->modifiers() & Qt::ShiftModifier);


  // Screen border Move
  if ((event->x() > 0.95*RenderInteractor.GetWindowSize()[0]
       || event->x() < 0.05*RenderInteractor.GetWindowSize()[0])
      && !(event->buttons() | Qt::NoButton))
  {
    RenderInteractor.MoveXDirectionScreen();
  }
  if ((event->y() > 0.95*RenderInteractor.GetWindowSize()[1]
       || event->y() < 0.05*RenderInteractor.GetWindowSize()[1])
      && !(event->buttons() | Qt::NoButton))
  {
    RenderInteractor.MoveZDirectionScreen();
  }


  // Left button controls the translation
  if (!ctrl_pressed && !shift_pressed && (event->buttons() & Qt::LeftButton) )
  {
    RenderInteractor.MoveZDirection();
    RenderInteractor.MoveXDirection();
  }

  // Right button controls the window rotation
  if (!ctrl_pressed && !shift_pressed && (event->buttons() & Qt::RightButton))
  {
    RenderInteractor.TrackBallRotateY();
  }

  // Shift+Left button controls the window zoom
  if (!ctrl_pressed && shift_pressed && (event->buttons() & Qt::LeftButton))
  {
    RenderInteractor.TrackBallZoomMouse();
  }

  // Shift+Right button enables to translate forward/backward
  if (!ctrl_pressed && shift_pressed && (event->buttons() & Qt::RightButton))
  {
    RenderInteractor.MoveUp();
  }

  //Store last cursor position
  RenderInteractor.SetLastEventPosition(event->x(),event->y());
}


void gltkApplicationWidgetGL::timerEvent(QTimerEvent *event)
{
  event->accept();
  update(); PRINT_OPENGL_ERROR();
}
//-------------------------------------------------------

//-------------------------------------------------------
//  GL callbacks
//-------------------------------------------------------
void gltkApplicationWidgetGL::initializeGL()
{
  //Init OpenGL
  InitGL();

  //Init Scene 3D
  RenderInteractor.Initialize();

  std::map<std::string, GLuint> textureBank;
 // textureBank["white.jpg"] = LoadTextureFromFile("white.jpg");
  //textureBank["hand_mapNew.jpg"] = LoadTextureFromFile("hand_mapNew.jpg");
//  textureBank["rooftiles.jpg"] = LoadTextureFromFile("rooftiles.jpg");
  //textureBank["UvMontantGauche.jpg"] = LoadTextureFromFile("UvMontantGauche.jpg");
//  textureBank["UvMontantPorte.jpg"] = LoadTextureFromFile("UvMontantPorte.jpg");
//  textureBank["UvporteSimple.jpg"] = LoadTextureFromFile("UvporteSimple.jpg");
//  textureBank["Uvpoutres.jpg"] = LoadTextureFromFile("Uvpoutres.jpg");
//  textureBank["UvTuiles.jpg"] = LoadTextureFromFile("UvTuiles.jpg");
//  textureBank["UvWall.jpg"] = LoadTextureFromFile("UvWall.jpg");
  textureBank["Grass.jpg"] = LoadTextureFromFile("Grass.jpg");
  textureBank["champ.jpg"] = LoadTextureFromFile("champ.jpg");
  textureBank["Argile.jpeg"] = LoadTextureFromFile("Argile.jpeg");
  textureBank["rock.jpg"] = LoadTextureFromFile("rock.jpg");
 // textureBank["stegosaurus.jpg"] = LoadTextureFromFile("stegosaurus.jpg");

  RenderInteractor.GetRenderManager().SetTextureBank(textureBank);

  //Activate depth buffer
  glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
  //Define provoking vertex for flat shading
  glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

  //Initialize timer
  TimeElapsed.start();
}

void gltkApplicationWidgetGL::resizeGL(int const width,int const height)
{
  RenderInteractor.SetWindowSize( width, height );
  glViewport(0,0, width, height); PRINT_OPENGL_ERROR();

}
//-------------------------------------------------------

//-------------------------------------------------------
//  Initialize OpenGL Context
//-------------------------------------------------------
void gltkApplicationWidgetGL::InitGL()
{
  //Print Context
  PrintGLInfo();
  PrintGLContext();

  InitGlew();
}

void gltkApplicationWidgetGL::InitGlew()
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

void gltkApplicationWidgetGL::PrintGLContext() const
{
  std::cout << "Current OpenGL context: " <<
               context()->format().majorVersion() << "." <<
               context()->format().minorVersion() << std::endl;
}
//-------------------------------------------------------

//-------------------------------------------------------
//  UI actions binding
//-------------------------------------------------------
void gltkApplicationWidgetGL::ToggleDrawState()
{
  DrawState =! DrawState;
  update(); PRINT_OPENGL_ERROR();
}
void gltkApplicationWidgetGL::Draw()
{
  DrawState =! DrawState;
  update(); PRINT_OPENGL_ERROR();
}

void gltkApplicationWidgetGL::SetWireframe(bool const wireframe)
{
  if (wireframe)
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);    PRINT_OPENGL_ERROR();
  }
  else
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);    PRINT_OPENGL_ERROR();
  }

  update(); PRINT_OPENGL_ERROR();
}
//-------------------------------------------------------

GLuint gltkApplicationWidgetGL::LoadTextureFromFile(std::string const& filename)
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


