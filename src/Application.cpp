#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "iostream"

double lastX = 0.f;
double lastY = 0.f;

bool firstMouse = true;
bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f ;

Application::Application() : GLApplication() 
{
  char cube_vs_path[] = "./src/shaders/cube.vs";
  char cube_fs_path[] =  "./src/shaders/cube.frag";
  this->m_cube_program = new Program(cube_vs_path, cube_fs_path);

  char light_vs_path[] = "./src/shaders/light.vs";
  char light_fs_path[] =  "./src/shaders/light.frag";
  this->m_light_program = new Program(light_vs_path, light_fs_path);

  this->camera = new Camera(glm::vec3(0.f, 0.f, 3.f));
}

Application::~Application()
{
  delete this->m_cube_program;
  delete this->m_light_program;
  delete this->camera;
}

void Application::startup()
{ 
  glEnable(GL_DEPTH_TEST);  

  GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  //bind vbo
  glGenBuffers(1, &this->m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //bind cube vao
  glGenVertexArrays(1, &this->m_cube_vao);
  glBindVertexArray(this->m_cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  glBindVertexArray(0); 

  //bind light vao
  glGenVertexArrays(1, &this->m_light_vao);
  glBindVertexArray(this->m_light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void Application::render(GLfloat time)
{
  deltaTime = time - lastFrame;
  lastFrame = time;

  this->do_camera_movement();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float light_x = sin(time) * 5;
  float light_y = sin(time) * 5;
  float light_z = cos(time) * 5;
  glm::vec3 light_pos(glm::vec3(light_x, light_y, light_z));

  this->m_cube_program->use();
  GLuint cur_program = this->m_cube_program->get_handle();

  glm::mat4 model;
  glm::mat4 view = this->camera->get_view_matrix();
  glm::mat4 projection = glm::perspective(camera->zoom, (float) (this->get_window_width() / this->get_window_height()), 0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(cur_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(cur_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(cur_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glUniform3f(glGetUniformLocation(cur_program, "objectColor"), 1.0f, 0.5f, 0.31f);
  glUniform3f(glGetUniformLocation(cur_program, "lightColor"),  1.0f, 1.f, 1.0f);
  glUniform3f(glGetUniformLocation(cur_program, "lightPos"),  light_pos.x, light_pos.y, light_pos.z);
  glUniform3f(glGetUniformLocation(cur_program, "viewPos"),  this->camera->position.x, this->camera->position.y, this->camera->position.z);

  glBindVertexArray(this->m_cube_vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glUseProgram(0);

  this->m_light_program->use();
  cur_program = this->m_light_program->get_handle();

  model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
  model = glm::translate(model, light_pos);

  glUniformMatrix4fv(glGetUniformLocation(cur_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(cur_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(cur_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray(this->m_light_vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glUseProgram(0);

  glfwSwapBuffers(this->get_window());
  glfwPollEvents();
}

void Application::shutdown()
{
    glDeleteVertexArrays(1, &this->m_cube_vao);
    glDeleteVertexArrays(1, &this->m_light_vao);
    glDeleteBuffers(1, &this->m_vbo);
}

void Application::do_camera_movement()
{
    camera->update_position(deltaTime);

    if(keys[GLFW_KEY_W])
        camera->process_keyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera->process_keyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera->process_keyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera->process_keyboard(RIGHT, deltaTime);
    if(keys[GLFW_KEY_SPACE])
        camera->process_keyboard(UP, deltaTime);
}

void Application::on_key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->get_window(), GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void Application::on_mouse_callback(double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    camera->process_mouse_movement(xoffset, yoffset);
}

void Application::on_scroll_callback(double xoffset, double yoffset)
{
    camera->process_mouse_scroll(yoffset);
}
