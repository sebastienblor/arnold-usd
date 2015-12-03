
#pragma once

#if defined(_DARWIN)
#include <OpenGL/gl.h>
#elif defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
// #include <GL/glew.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif


/* Print OpenGL error messages */
void PrintGLError(const char *operation);

void PrintGPUState();



