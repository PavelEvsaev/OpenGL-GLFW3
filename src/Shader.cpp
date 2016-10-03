#include "Shader.h"
#include "stdio.h"
#include "stdlib.h"

char* fileToBuffer(char *file)
{
    FILE *fptr = fopen(file, "rb");

    if (!fptr)
    {
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    long length = ftell(fptr);
    char *buf = (char*)malloc(length+1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

Shader::Shader(GLenum type, char* location)
{
  m_handle = glCreateShader(type);

  GLchar* src = fileToBuffer(location);
  glShaderSource(m_handle, 1, (const GLchar**)&src, NULL);
  glCompileShader(m_handle);

  if (status() == GL_FALSE)
  {
    log_error(location);
  }
}

GLuint Shader::get_handle()
{
  return m_handle;
}

GLint Shader::status()
{
  GLint status;
  glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);

  return status;
}

void Shader::attach_to(GLuint programId)
{
  glAttachShader(programId, m_handle);
}

void Shader::mark_to_delete()
{
  glDeleteShader(m_handle);
}

void Shader::log_error(char* location)
{
  GLint infoLogLength;
  glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);

  GLchar *strInfoLog = new GLchar[infoLogLength + 1];
  glGetShaderInfoLog(m_handle, infoLogLength, NULL, strInfoLog);

  fprintf(stderr, "Shader Compile failure in %s:\n%s\n", location, strInfoLog);
  delete[] strInfoLog;
}