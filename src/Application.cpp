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
    // vertices           //colors
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f
  };

  GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
  };  

  char vs_path[] = "./src/shaders/vertex_shader.vs";
  char fs_path[] =  "./src/shaders/fragment_shader.frag";
  this->m_program = compile_shaders(vs_path, fs_path);

  glGenBuffers(1, &this->m_vbo);
  glGenBuffers(1, &this->m_ebo);
  glGenVertexArrays(1, &this->m_vao);

  //init vao
  glBindVertexArray(this->m_vao);
    //init vbo
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //setup vertex position att
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  
    //setup vertex color att
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

  glBindVertexArray(0); 
}

void Application::render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  GLint xOffsetLocation = glGetUniformLocation(this->m_program, "xOffset");

  glUseProgram(this->m_program);
  glBindVertexArray(this->m_vao);
  glUniform1f(xOffsetLocation, 0.2f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
