#pragma once

#include "Shader.h"

class Program : IGLErrorPrint
{
private:
	GLuint m_handle;
	Shader *m_vs;
	Shader *m_fs;

public:
	Program(char *vs_path, char *fs_path);
	~Program();

	GLuint get_handle();
	void use();

private:
	GLint get_status() override;
    void print_error(char *) override;
};