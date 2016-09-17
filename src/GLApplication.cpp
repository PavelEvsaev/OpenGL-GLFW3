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

GLFWwindow* GLApplication::create_main_window(int w, int h)
{
  GLFWwindow *window = glfwCreateWindow(w, h, "OpenGL", NULL, NULL);

  if(!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  return window;
}

GLApplication::GLApplication(int w, int h)
{
  init_glfw();

  m_window = create_main_window(w, h);
  glfwMakeContextCurrent(m_window);

  init_glew();
}

GLApplication::~GLApplication()
{
  glfwTerminate();
}

void GLApplication::run()
{
  this->startup();

  while(!glfwWindowShouldClose(m_window))
  {
    this->render();
  }

  this->shutdown();
}

GLFWwindow* GLApplication::get_window() const
{
  return m_window;
}

void GLApplication::render() 
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void GLApplication::startup()
{  
}

void GLApplication::shutdown()
{
}