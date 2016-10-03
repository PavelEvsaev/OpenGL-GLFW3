#include "Program.h"

Program::Program(char *vs_path, char *fs_path)
{
	this->m_vs = new Shader(GL_VERTEX_SHADER, vs_path);
	this->m_fs = new Shader(GL_FRAGMENT_SHADER, fs_path);

	this->m_handle = glCreateProgram();
	this->m_vs->attach_to(this->m_handle);
	this->m_fs->attach_to(this->m_handle);
	glLinkProgram(this->m_handle);

	//shaders will be finally deleted after glDeleteProgram() call
	this->m_vs->mark_to_delete();
	this->m_fs->mark_to_delete();
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