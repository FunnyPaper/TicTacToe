#pragma once

#include <glad\glad.h>
// Makro
// Jeœli argument po negacji zwraca prawdê przerwij dzia³anie programu w miejscu wywo³ania
// __debugbreak to specjalna funkcja VS do przerywania programu
#define ASSERT(x) if (!(x)) __debugbreak();

// Jeœli projekt jest w trybie DEBUG to zdefiniuj makro GLCall(x)
// wywo³aj funkcjê GL_EmptyErrorStack a nastêpnie wykorzystaj makro ASSERT w po³¹czeniu z funkcj¹ GL_ErrorCallBack
#ifdef DEBUG 
#define GLCall(x) GL_EmptyErrorStack();\
ASSERT(GL_ErroCallback(#x, __FILE__, __LINE__));\
x
#else
// W przeciwnym wypadku GLCall nie robi nic
#define GLCall(x) x;
#endif // DEBUG 

// Wyczyœæ kolejkê b³êdów
void GL_EmptyErrorStack();
// Z³ap wszystkie b³êdy OpenGL i wypisz je w konsoli
bool GL_ErroCallback(const char* functionName, const char* file, int line);
