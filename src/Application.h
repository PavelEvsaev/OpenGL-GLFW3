#pragma once

#include "GLApplication.h"

class Application : public GLApplication
{
private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_texture;
	GLuint m_program;
	
	GLuint compile_shaders(char *, char *);
	void do_camera_movement();

protected:
  	void render() override;
  	void startup() override;
  	void shutdown() override;

  	//callbacks
  	void init_callbacks();
  	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:
	Application();
};
