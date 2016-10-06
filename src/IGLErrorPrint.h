#pragma once 

#include <GL/glew.h>
#include "stdio.h"

class IGLErrorPrint
{
	virtual GLint get_status() = 0;
    virtual void print_error(char *) = 0;
};