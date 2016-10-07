#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"

Application::Application() : GLApplication() 
{
  char vs_path[] = "./src/shaders/shader.vs";
  char fs_path[] =  "./src/shaders/shader.frag";

  this->m_program = std::make_shared<Program>(vs_path, fs_path);
  this->m_camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f));
  this->m_model = std::make_shared<Model>("./src/model/nanosuit.obj");
}

void Application::startup()
{ 
  glEnable(GL_DEPTH_TEST);
}

void Application::render(GLfloat cur_time, GLfloat last_time_render)
{
  glfwPollEvents();

  glClearColor(0.f, 0.f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->do_camera_movement(cur_time - last_time_render);

  glm::mat4 model;
  model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); 
  glm::mat4 projection = glm::perspective(m_camera->zoom, (float)(this->get_window_width()/this->get_window_height()), 0.1f, 100.0f);
  glm::mat4 mvp = projection * m_camera->get_view_matrix() * model;

  this->m_program->use();
  GLuint program_id = this->m_program->get_handle();

  glUniformMatrix4fv(glGetUniformLocation(program_id, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
  glUniformMatrix4fv(glGetUniformLocation(program_id, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

  float posx = this->m_camera->position.x;
  float posy = this->m_camera->position.y;
  float posz = this->m_camera->position.z;

  glUniform3f(glGetUniformLocation(program_id, "viewPos"), posx, posy, posz);
  glUniform3f(glGetUniformLocation(program_id, "light.position"),  posx, posy, posz);
  glUniform3f(glGetUniformLocation(program_id, "light.direction"), m_camera->front.x, m_camera->front.y, m_camera->front.z);
  
  glUniform3f(glGetUniformLocation(program_id, "light.ambient"),   0.8f, 0.8f, 0.8f);
  glUniform3f(glGetUniformLocation(program_id, "light.diffuse"),   0.8f, 0.8f, 0.8f);
  glUniform3f(glGetUniformLocation(program_id, "light.specular"),  1.0f, 1.0f, 1.0f);

  glUniform1f(glGetUniformLocation(program_id, "light.outerCutOff"), cos(glm::radians(20.f)));
  glUniform1f(glGetUniformLocation(program_id, "light.innerCutOff"), cos(glm::radians(10.f)));

  glUniform1f(glGetUniformLocation(program_id, "light.constant"),  1.0f);
  glUniform1f(glGetUniformLocation(program_id, "light.linear"),    0.09);
  glUniform1f(glGetUniformLocation(program_id, "light.quadratic"), 0.032);

  this->m_model->draw(this->m_program);

  glfwSwapBuffers(this->get_window());
}

void Application::do_camera_movement(GLfloat delta_time)
{
    this->m_camera->update_position(delta_time);

    if(this->pressed_keys[GLFW_KEY_W])
        this->m_camera->process_keyboard(FORWARD, delta_time);
    if(this->pressed_keys[GLFW_KEY_S])
        this->m_camera->process_keyboard(BACKWARD, delta_time);
    if(this->pressed_keys[GLFW_KEY_A])
        this->m_camera->process_keyboard(LEFT, delta_time);
    if(this->pressed_keys[GLFW_KEY_D])
        this->m_camera->process_keyboard(RIGHT, delta_time);
    if(this->pressed_keys[GLFW_KEY_SPACE])
        this->m_camera->process_keyboard(UP, delta_time);
}

void Application::on_mouse_callback(double xpos, double ypos, float dx, float dy)
{
    m_camera->process_mouse_movement(dx, dy);
}

void Application::on_scroll_callback(double xoffset, double yoffset)
{
    m_camera->process_mouse_scroll(yoffset);
}
