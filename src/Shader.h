#pragma once

#include <GL/glew.h>

class Shader
{
  public:

    Shader(GLenum shaderType, char* shaderLocation);

    GLuint getHandle();
    GLint status();

    void attachTo(GLuint programId);
    void detachFrom(GLuint programId);
    
  private:

    GLuint m_handle;
    void logError(char*);
};