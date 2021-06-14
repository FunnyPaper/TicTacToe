#pragma once

#include <glad\glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG 
#define GLCall(x) GL_EmptyErrorStack();\
ASSERT(GL_ErroCallback(#x, __FILE__, __LINE__));\
x
#else
#define GLCall(x) x;
#endif // DEBUG 

void GL_EmptyErrorStack();
bool GL_ErroCallback(const char* functionName, const char* file, int line);
