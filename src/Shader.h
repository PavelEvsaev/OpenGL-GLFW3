#pragma once

#include <GL/glew.h>

class Shader
{
public:
    Shader(GLenum shader_type, char* shader_location);

    GLuint get_handle();
    GLint status();

    void attach_to(GLuint program_id);
	void mark_to_delete();

private:
    GLuint m_handle;
    void log_error(char*);
};