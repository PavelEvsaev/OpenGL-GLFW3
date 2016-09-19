#pragma once

#include "GLApplication.h"

class Application : public GLApplication
{
private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_texture1;
	GLuint m_texture2;
	GLuint m_program;
	
	GLuint compile_shaders(char *, char *);

protected:
  	void render() override;
  	void startup() override;
  	void shutdown() override;

  	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
