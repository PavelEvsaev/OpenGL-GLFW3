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

	Camera  *camera;
	Program *m_cube_program;
	Program *m_light_program;

	void do_camera_movement(GLfloat delta_time);

protected:
	void render(GLfloat cur_time, GLfloat last_time_render) override;
  	void startup() override;
  	void shutdown() override;
  	
  	void on_scroll_callback(double xoffset, double yoffset) override;
  	void on_mouse_callback(double xpos, double ypos, float dx, float dy) override;

public:
	Application();
	~Application();
};
