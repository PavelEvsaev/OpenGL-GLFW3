#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "iostream"

Application::Application() : GLApplication() 
{
  char cube_vs_path[] = "./src/shaders/cube.vs";
  char cube_fs_path[] =  "./src/shaders/cube.frag";
  this->m_cube_program = new Program(cube_vs_path, cube_fs_path);
  
  this->camera = new Camera(glm::vec3(0.f, 0.f, 3.f));
}

Application::~Application()
{
  delete this->m_cube_program;
  delete this->camera;
}

void Application::startup()
{ 
  glEnable(GL_DEPTH_TEST);  

  GLfloat vertices[] = {
    // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };

  //bind vbo
  glGenBuffers(1, &this->m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //bind cube vao
  glGenVertexArrays(1, &this->m_cube_vao);
  glBindVertexArray(this->m_cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
  glBindVertexArray(0); 

  int width, height;
  unsigned char* image;
  image = SOIL_load_image("container.png", &width, &height, 0, SOIL_LOAD_RGB);
  glGenTextures(1, &this->m_deffuse_texture);
  glBindTexture(GL_TEXTURE_2D, this->m_deffuse_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image); 

  image = SOIL_load_image("container_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
  glGenTextures(1, &this->m_specular_texture);
  glBindTexture(GL_TEXTURE_2D, this->m_specular_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
}

void Application::render(GLfloat cur_time, GLfloat last_time_render)
{
  glfwPollEvents();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->do_camera_movement(cur_time - last_time_render);

  glm::vec3 light_pos = this->camera->position;
  glm::vec3 light_dir = this->camera->front;

  this->m_cube_program->use();
  GLuint cur_program = this->m_cube_program->get_handle();

  glm::mat4 model;
  glm::mat4 view = this->camera->get_view_matrix();
  glm::mat4 projection = glm::perspective(camera->zoom, (float) (this->get_window_width() / this->get_window_height()), 0.1f, 100.0f);
  glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));
  glm::mat4 mvp = projection * view * model;

  glUniformMatrix3fv(glGetUniformLocation(cur_program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normal));
  glUniformMatrix4fv(glGetUniformLocation(cur_program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(cur_program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

  glUniform1i(glGetUniformLocation(cur_program, "material.diffuse"),  0);
  glUniform1i(glGetUniformLocation(cur_program, "material.specular"), 1);
  glUniform3f(glGetUniformLocation(cur_program, "material.specular"), 0.5f, 0.5f, 0.5f);
  glUniform1f(glGetUniformLocation(cur_program, "material.shininess"), 32.0f);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->m_deffuse_texture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, this->m_specular_texture);
  
  glUniform3f(glGetUniformLocation(cur_program, "light.position"), light_pos.x, light_pos.y, light_pos.z);
  glUniform3f(glGetUniformLocation(cur_program, "light.direction"), light_dir.x, light_dir.y, light_dir.z);

  glUniform3f(glGetUniformLocation(cur_program, "light.ambient"),  0.2f, 0.2f, 0.2f);
  glUniform3f(glGetUniformLocation(cur_program, "light.diffuse"),  0.5f, 0.5f, 0.5f);
  glUniform3f(glGetUniformLocation(cur_program, "light.specular"), 1.0f, 1.0f, 1.0f);

  glUniform1f(glGetUniformLocation(cur_program, "light.quadratic"), 0.032);
  glUniform1f(glGetUniformLocation(cur_program, "light.constant"),  1.0f);
  glUniform1f(glGetUniformLocation(cur_program, "light.linear"),    0.09);

  glUniform1f(glGetUniformLocation(cur_program, "light.outerCutOff"), cos(glm::radians(15.f)));
  glUniform1f(glGetUniformLocation(cur_program, "light.innerCutOff"), cos(glm::radians(10.f)));

  glUniform3f(glGetUniformLocation(cur_program, "viewPos"),  this->camera->position.x, this->camera->position.y, this->camera->position.z);

  glBindVertexArray(this->m_cube_vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glUseProgram(0);

  glfwSwapBuffers(this->get_window());
}

void Application::shutdown()
{
    glDeleteVertexArrays(1, &this->m_cube_vao);
    glDeleteBuffers(1, &this->m_vbo);
}

void Application::do_camera_movement(GLfloat delta_time)
{
    this->camera->update_position(delta_time);
    if(this->pressed_keys[GLFW_KEY_W])
        this->camera->process_keyboard(FORWARD, delta_time);
    if(this->pressed_keys[GLFW_KEY_S])
        this->camera->process_keyboard(BACKWARD, delta_time);
    if(this->pressed_keys[GLFW_KEY_A])
        this->camera->process_keyboard(LEFT, delta_time);
    if(this->pressed_keys[GLFW_KEY_D])
        this->camera->process_keyboard(RIGHT, delta_time);
    if(this->pressed_keys[GLFW_KEY_SPACE])
        this->camera->process_keyboard(UP, delta_time);
}

void Application::on_mouse_callback(double xpos, double ypos, float dx, float dy)
{
    camera->process_mouse_movement(dx, dy);
}

void Application::on_scroll_callback(double xoffset, double yoffset)
{
    camera->process_mouse_scroll(yoffset);
}
