#version 330 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_color;
layout (location = 2) in vec2 vs_texture;
layout (location = 3) in vec3 vs_normal;

// Transformacje obiektu 
uniform mat4 model; // przesuniecie obiektu wzgledem rzutowania i widoku
uniform mat4 view; // widok
uniform mat4 projection; // rzutowanie
uniform mat3 model_normals; // do aktualizowania vs_normal -> gdy MVP ulega zmianie w glownej petli programu

// Przekazywane dalej do fragment shader
out vec3 fs_color; // kolory 
out vec2 fs_texture; // pozycje tekstur (UV)
out vec3 fs_normal; // wektory bedace wynikiem cross product
out vec3 fs_position; // rozmieszczenie wierzcholku w przestrzeni

void main()
{
	// Predefiniowana zmienna gl_Position odpowiada za rozmieszczenie fragmentow w przestrzeni
	gl_Position = projection * view * model * vec4(vs_position, 1.0f);
	// Przekaz kolor, pozycje tekstur, znormalizowane wektory (zmodyfikowane o znormalizowana macierz modelu) 
	// oraz pozycje fragmentow (odpowiednio przeksztalconych przez macierz modelu) 
	fs_color = vs_color;
	fs_texture = vs_texture;
	fs_normal = vs_normal * model_normals; // przeksztalc oryginalne znormalizowane wektory tak by odzwierciedlaly zmiany zachodzace w proporcjach obiektu
	fs_position = vec3(model * vec4(vs_position, 1.0f)); // przekaz pozycje wierzcholkow 
}