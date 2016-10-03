#pragma once

#include "Shader.h"
#include <GL/glew.h>

class Program
{
public:
	Program(char *vs_path, char *fs_path);
	~Program();

	void use();
	GLuint get_handle();

private:
	GLuint m_handle;
	Shader *m_vs;
	Shader *m_fs;
};