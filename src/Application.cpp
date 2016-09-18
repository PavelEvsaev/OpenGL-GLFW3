#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#include "Shader.h"
#include "Application.h"
#include "iostream"

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
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  GLfloat vertices[] = {
    // Positions          // Colors           // Texture Coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f   // Bottom Left
  };

  GLuint indices[] = {
    0, 1, 2
  };  

  char vs_path[] = "./src/shaders/vertex_shader.vs";
  char fs_path[] =  "./src/shaders/fragment_shader.frag";
  this->m_program = compile_shaders(vs_path, fs_path);

  glGenBuffers(1, &this->m_vbo);
  glGenBuffers(1, &this->m_ebo);
  glGenTextures(1, &this->m_texture);
  glGenVertexArrays(1, &this->m_vao);

  //init vao
  glBindVertexArray(this->m_vao);
    //bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //bind ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //setup vertex position att
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  
    //setup vertex color att
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //setup vertex texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
  //unbind vao
  glBindVertexArray(0); 

  //load textures
  int width, height;
  unsigned char* image = SOIL_load_image(
          "wall.jpg",
          &width,
          &height, 
          0, 
          SOIL_LOAD_RGB
  );
  //bind textures
  glBindTexture(GL_TEXTURE_2D, this->m_texture); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
}

void Application::render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, this->m_texture);

  glUseProgram(this->m_program);
  glBindVertexArray(this->m_vao);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glfwSwapBuffers(GLApplication::get_window());
  glfwPollEvents();
}

void Application::shutdown()
{
  glDeleteVertexArrays(1, &this->m_vao);
  glDeleteBuffers(1, &this->m_vbo);
  glDeleteBuffers(1, &this->m_ebo);
  glDeleteProgram(this->m_program);
  
  glfwTerminate();
}
