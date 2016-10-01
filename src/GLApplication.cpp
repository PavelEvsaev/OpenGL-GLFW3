#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "GLApplication.h"

void GLApplication::init_glfw()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Failed initialize GLFW.");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void GLApplication::init_glew()
{
  if (glewInit() != GLEW_OK)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
}

void GLApplication::init_window(int w, int h)
{
  m_window = glfwCreateWindow(w, h, "OpenGL", NULL, NULL);
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  

  if(!m_window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_window);
}

GLApplication::GLApplication(int w, int h)
{
  init_glfw();
  init_window(w, h);
  init_glew();
}

GLApplication::~GLApplication()
{
  glfwTerminate();
}

void GLApplication::run()
{
  this->startup();

  while(!glfwWindowShouldClose(this->m_window))
  {
    this->render();
  }

  this->shutdown();
}

GLFWwindow* GLApplication::get_window() const
{
  return m_window;
}

int GLApplication::get_window_height()
{
  int width, height;
  glfwGetWindowSize(this->m_window, &width, &height);
  return height;
}

int GLApplication::get_window_width()
{
  int width, height;
  glfwGetWindowSize(this->m_window, &width, &height);
  return width;
}

void GLApplication::render() 
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(this->m_window);
  glfwPollEvents();
}

void GLApplication::startup()
{  
}

void GLApplication::shutdown()
{
}