#pragma once

#include <GLFW/glfw3.h>

class GLApplication
{
private:

  GLFWwindow* m_window;

  void init_glfw();
  void init_glew();
  void init_window(int w, int h);

public:

  GLApplication(int w = 650, int h = 600);
  ~GLApplication();

  void run();

protected:

  GLFWwindow* get_window() const;

  int get_window_height();
  int get_window_width();

  //application lifecycle
  virtual void render();
  virtual void startup();
  virtual void shutdown();
};