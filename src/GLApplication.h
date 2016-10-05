#pragma once

#include <GLFW/glfw3.h>
#include <map>

class GLApplication
{
private:

  static GLApplication *current_app;

  GLFWwindow* m_window;

  void init_glfw();
  void init_glew();
  void init_window(int w, int h);
  void init_callbacks();

  static void error_callback(int error, const char* description);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:

  GLApplication(int w = 650, int h = 600);
  virtual ~GLApplication();

  void run(GLApplication *the_app);

protected:

  GLFWwindow* get_window() const;
  std::map<int, bool> pressed_keys;

  int get_window_height();
  int get_window_width();

  //application lifecycle
  virtual void render(GLfloat cur_time, GLfloat last_time_render);
  virtual void startup();
  virtual void shutdown();

  //application callbacks
  virtual void on_key_callback(int key, int scancode, int action, int mods);
  virtual void on_scroll_callback(double xoffset, double yoffset);
  virtual void on_mouse_callback(double xpos, double ypos, float dx, float dy);
};