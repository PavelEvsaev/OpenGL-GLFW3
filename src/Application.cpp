#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
  GLfloat vertices[] = {
    // Positions          // Colors           // Texture Coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f    // Top Left
  };

  GLuint indices[] = {
    0, 1, 2,
    2, 3, 0
  };  

  char vs_path[] = "./src/shaders/vertex_shader.vs";
  char fs_path[] =  "./src/shaders/fragment_shader.frag";
  this->m_program = compile_shaders(vs_path, fs_path);

  glGenBuffers(1, &this->m_vbo);
  glGenBuffers(1, &this->m_ebo);
  glGenTextures(1, &this->m_texture1);
  glGenTextures(1, &this->m_texture2);
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
  unsigned char* image1 = SOIL_load_image(
          "container.jpg",
          &width,
          &height, 
          0,
          SOIL_LOAD_RGB
  );
  //bind textures
  glBindTexture(GL_TEXTURE_2D, this->m_texture1); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image1);

  unsigned char* image2 = SOIL_load_image(
          "smile.png",
          &width,
          &height, 
          0,
          SOIL_LOAD_RGB
  );

  glBindTexture(GL_TEXTURE_2D, this->m_texture2); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image2);
}

void Application::render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(this->m_program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->m_texture1);
  glUniform1i(glGetUniformLocation(this->m_program, "ourTexture1"), 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, this->m_texture2);
  glUniform1i(glGetUniformLocation(this->m_program, "ourTexture2"), 1);

  glm::mat4 transform;
  transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.0f));

  GLint transformLoc = glGetUniformLocation(this->m_program, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
  glBindVertexArray(this->m_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);

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
