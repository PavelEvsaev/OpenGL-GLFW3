#include "Shader.h"
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

  if (get_status() == GL_FALSE)
  {
    print_error(location);
  }
}

GLuint Shader::get_handle()
{
  return m_handle;
}

GLint Shader::get_status()
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

void Shader::print_error(char* info)
{
  GLint log_length;
  glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);

  GLchar *log = new GLchar[log_length + 1];
  glGetShaderInfoLog(m_handle, log_length, NULL, log);

  fprintf(stderr, "Shader Compile failure in %s:\n%s\n", info, log);
  delete[] log;
}