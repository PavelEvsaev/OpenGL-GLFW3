#pragma once

#include "IGLErrorPrint.h"

class Shader : IGLErrorPrint
{
private:
    GLuint m_handle;

public:
    Shader(GLenum shader_type, char* shader_location);

    GLuint get_handle();

    void attach_to(GLuint program_id);
	void mark_to_delete();

private:
    GLint get_status() override;
    void print_error(char *) override;
};