#pragma once

#include <GLFW/glfw3.h>

class GLApplication
{
private:

  GLFWwindow* m_window;

  void init_glfw();
  void init_glew();
  
  GLFWwindow* create_main_window(int w = 800, int h = 600);

public:

  GLApplication(int w = 800, int h = 600);
  ~GLApplication();

  void run();

protected:

  GLFWwindow* get_window() const;

  virtual void render();
  virtual void startup();
  virtual void shutdown();
};