#pragma once

#include <memory>

#include "Camera.h"
#include "Model.h"
#include "GLApplication.h"

class Application : public GLApplication
{
private:
	std::shared_ptr<Program> m_program;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<Model> m_model;

	void do_camera_movement(GLfloat delta_time);

protected:
	void render(GLfloat cur_time, GLfloat last_time_render) override;
  	void startup() override;
  	
  	void on_scroll_callback(double xoffset, double yoffset) override;
  	void on_mouse_callback(double xpos, double ypos, float dx, float dy) override;

public:
	Application();
};
