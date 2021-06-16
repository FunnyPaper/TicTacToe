#pragma once

#include <glad\glad.h>
// Makro
// Je�li argument po negacji zwraca prawd� przerwij dzia�anie programu w miejscu wywo�ania
// __debugbreak to specjalna funkcja VS do przerywania programu
#define ASSERT(x) if (!(x)) __debugbreak();

// Je�li projekt jest w trybie DEBUG to zdefiniuj makro GLCall(x)
// wywo�aj funkcj� GL_EmptyErrorStack a nast�pnie wykorzystaj makro ASSERT w po��czeniu z funkcj� GL_ErrorCallBack
#ifdef DEBUG 
#define GLCall(x) GL_EmptyErrorStack();\
ASSERT(GL_ErroCallback(#x, __FILE__, __LINE__));\
x
#else
// W przeciwnym wypadku GLCall nie robi nic
#define GLCall(x) x;
#endif // DEBUG 

// Wyczy�� kolejk� b��d�w
void GL_EmptyErrorStack();
// Z�ap wszystkie b��dy OpenGL i wypisz je w konsoli
bool GL_ErroCallback(const char* functionName, const char* file, int line);
