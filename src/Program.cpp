#include "Program.h"

Program::Program(char *vs_path, char *fs_path)
{
	this->m_vs = new Shader(GL_VERTEX_SHADER, vs_path);
	this->m_fs = new Shader(GL_FRAGMENT_SHADER, fs_path);

	this->m_handle = glCreateProgram();
	this->m_vs->attach_to(this->m_handle);
	this->m_fs->attach_to(this->m_handle);
	glLinkProgram(this->m_handle);

	if (this->get_status() == GL_FALSE)
	{
		this->print_error(NULL);
	}

	//shaders will be finally deleted after glDeleteProgram() call
	this->m_vs->mark_to_delete();
	this->m_fs->mark_to_delete();
}

GLint Program::get_status()
{	
	GLint status;
  	glGetProgramiv(m_handle, GL_LINK_STATUS, &status);

  	return status;
}

void Program::print_error(char *info)
{
	GLint log_length;
  	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);

  	GLchar *log = new GLchar[log_length + 1];
  	glGetProgramInfoLog(m_handle, log_length, NULL, log);

  	fprintf(stderr, "Program link failure: %s\n",log);
  	delete[] log;
}

Program::~Program()
{	
	glDeleteProgram(this->m_handle);
	delete this->m_vs;
	delete this->m_fs;
}

GLuint Program::get_handle()
{
	return this->m_handle;
}

void Program::use()
{	
	glUseProgram(this->m_handle);
}
