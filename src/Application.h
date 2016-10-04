#pragma once

#include "Program.h"
#include "Camera.h"
#include "GLApplication.h"

class Application : public GLApplication
{
private:
	GLuint m_vbo;
	GLuint m_cube_vao;
	GLuint m_light_vao;

	GLuint m_deffuse_texture;
	GLuint m_specular_texture;

	Camera *camera;
	Program *m_cube_program;
	Program *m_light_program;

	void do_camera_movement();

protected:
  	void render(GLfloat time) override;
  	void startup() override;
  	void shutdown() override;

  	void on_key_callback(int key, int scancode, int action, int mods) override;
  	void on_scroll_callback(double xoffset, double yoffset) override;
  	void on_mouse_callback(double xpos, double ypos) override;

public:
	Application();
	~Application();
};
