#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Shader.h"
#include "Application.h"
#include "Camera.h"
#include "iostream"

Camera camera(glm::vec3(0.f, 0.f, 3.f));

double lastX = 400.f;
double lastY = 300.f;

bool firstMouse = true;
bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Application::Application() : GLApplication() 
{
  init_callbacks();
}

void Application::init_callbacks()
{
  glfwSetKeyCallback(this->get_window(), Application::key_callback);
  glfwSetCursorPosCallback(this->get_window(), Application::mouse_callback);
  glfwSetScrollCallback(this->get_window(), Application::scroll_callback);
}

GLuint Application::compile_shaders(char *vs_path, char *fs_path)
{
  Shader vs = Shader(GL_VERTEX_SHADER, vs_path);
  Shader fs = Shader(GL_FRAGMENT_SHADER, fs_path);
  GLuint program = glCreateProgram();
  vs.attachTo(program);
  fs.attachTo(program);
 
  glLinkProgram(program);

  return program;
}

void Application::startup()
{ 
  glEnable(GL_DEPTH_TEST);  

  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  char vs_path[] = "./src/shaders/vertex_shader.vs";
  char fs_path[] =  "./src/shaders/fragment_shader.frag";
  this->m_program = compile_shaders(vs_path, fs_path);

  glGenBuffers(1, &this->m_vbo);
  glGenTextures(1, &this->m_texture);
  glGenVertexArrays(1, &this->m_vao);

  //init vao
  glBindVertexArray(this->m_vao);
    //bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //setup vertex position att
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  
    //setup vertex texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  //unbind vao
  glBindVertexArray(0); 

  //load textures
  int width, height;
  unsigned char* image1 = SOIL_load_image(
          "container.jpg",
          &width,
          &height, 
          0,
          SOIL_LOAD_RGB
  );

  glBindTexture(GL_TEXTURE_2D, this->m_texture); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image1);
}

void Application::render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(this->m_program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->m_texture);
  glUniform1i(glGetUniformLocation(this->m_program, "ourTexture"), 0);

  glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  GLfloat currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  glm::mat4 view = camera.get_view_matrix();

  glm::mat4 projection;
  float aspect_ratio = (float) (this->get_window_width() / this->get_window_height());
  projection = glm::perspective(camera.zoom, aspect_ratio, 0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(this->m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(this->m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  this->do_camera_movement();

  for (int i = 0; i < 10; i++)
  {
    glm::vec3 translation = cube_positions[i];

    glm::mat4 model;
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::translate(model, translation);

    glUniformMatrix4fv(glGetUniformLocation(this->m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(this->m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  glfwSwapBuffers(GLApplication::get_window());
  glfwPollEvents();
}

void Application::shutdown()
{
  glDeleteVertexArrays(1, &this->m_vao);
  glDeleteBuffers(1, &this->m_vbo);
  glDeleteProgram(this->m_program);

  glfwTerminate();
}

void Application::do_camera_movement()
{
    camera.update_position(deltaTime);

    if(keys[GLFW_KEY_W])
        camera.process_keyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.process_keyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.process_keyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.process_keyboard(RIGHT, deltaTime);
    if(keys[GLFW_KEY_SPACE])
        camera.process_keyboard(UP, deltaTime);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
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

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    camera.process_mouse_movement(xoffset, yoffset);
}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.process_mouse_scroll(yoffset);
}
