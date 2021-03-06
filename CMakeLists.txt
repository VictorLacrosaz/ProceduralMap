#CMakeLists.txt to build project using cmake
#run $cmake CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

project( ProceduralMapGeneration )

# Configure Project
set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

set( SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set( DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data" )
set( CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake" )
set( SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shader" )

configure_file( ${CMAKE_DIR}/ProjectConfig.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/ProjectConfig.h @ONLY )
  
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_DIR} )

# Find external packages
find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost 1.54 REQUIRED)

set(QT_USE_OPENGL TRUE)
include(${QT_USE_FILE})

# Find executable sources
file( GLOB_RECURSE source_files
  ${CMAKE_CURRENT_SOURCE_DIR}/*.[cht]pp
  ${SHADER_DIR}/*.frag
  ${SHADER_DIR}/*.vert )

# Include directories
include(ProjectConfig)
include_directories(${GLTK_SRC_DIRS} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

# Specify g++ option
add_definitions( -Wall -Wextra -std=c++11 -Wno-comment -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -DCORRECTION)

# UI
set( UI ${SRC_DIR}/Application/gltkApplicationWindow.ui )
set( MOC ${SRC_DIR}/Application/gltkApplicationQt.hpp
         ${SRC_DIR}/Application/gltkApplicationWindow.hpp
         ${SRC_DIR}/Application/gltkApplicationWidgetGL.hpp)

QT4_WRAP_CPP( MOC_GENERATED ${MOC} )
QT4_WRAP_UI( UI_GENERATED ${UI} )

# Target
add_executable( ${PROJECT_NAME}
  ${source_files}
  ${UI_GENERATED}
  ${MOC_GENERATED} )

target_link_libraries( ${PROJECT_NAME} -lm -ldl -lGLEW ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES} ${QT_GL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} -lboost_serialization ${Boost_LIBRARIES})
