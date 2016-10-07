#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "GLApplication.h"

GLApplication* GLApplication::current_app = nullptr;

GLApplication::GLApplication(int w, int h)
{
  init_glfw();
  init_window(w, h);
  init_glew();
  init_callbacks();
}

GLApplication::~GLApplication()
{
  glfwTerminate();
}

void GLApplication::init_glfw()
{
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
  {
    fprintf(stderr, "Failed initialize GLFW.");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void GLApplication::init_glew()
{
  glewExperimental = GL_TRUE; 

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
  glViewport(0, 0, w, h);  

  if(!m_window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_window);
}

void GLApplication::init_callbacks()
{
  glfwSetKeyCallback(m_window, key_callback);
  glfwSetCursorPosCallback(m_window, mouse_callback);
  glfwSetScrollCallback(m_window, scroll_callback);
}

void GLApplication::run(GLApplication *the_app)
{
  current_app = the_app;

  GLfloat last_time_render = 0.0f;
  GLfloat current_time = 0.0f;

  this->startup();

  while(!glfwWindowShouldClose(this->m_window))
  {
    current_time = glfwGetTime();
    this->render(current_time, last_time_render);
    last_time_render = current_time;
  }

  this->shutdown();
}

void GLApplication::startup()
{  
}

void GLApplication::render(GLfloat cur_time, GLfloat last_time_render)
{
  glfwPollEvents();
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(this->m_window);
}

void GLApplication::shutdown()
{
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

void GLApplication::on_key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->get_window(), GL_TRUE);
    }

    if (action == GLFW_PRESS)
    {
        this->pressed_keys[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        this->pressed_keys[key] = false;
    }
}

void GLApplication::on_mouse_callback(double xpos, double ypos, float dx, float dy)
{
}

void GLApplication::on_scroll_callback(double xoffset, double yoffset)
{
}

void GLApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    current_app->on_key_callback(key, scancode, action, mods);
}

void GLApplication::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX = 0.f;
    static double lastY = 0.f;

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    current_app->on_mouse_callback(xpos, ypos, xoffset, yoffset);
}

void GLApplication::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    current_app->on_scroll_callback(xoffset, yoffset);
}

void GLApplication::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}