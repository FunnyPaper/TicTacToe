#pragma once
#include "Player.h"
#include "../Mesh.h"
#include <glm\glm.hpp>
#include "../Shader.h"
#include "../Camera.h"
#include <GLFW\glfw3.h>

class Board
{
    // Klasa odpowiedzialna za rysowanie planszy i proces gry (kt�ry gracz powinien wykona� ruch)
private:
    // Wska�nik na okno kontekstowe
    GLFWwindow* _window;
    // Wska�nik na pojedyncze pole
    Mesh *_tile;
    // Wska�nik na shader stosowany do renderowania grafiki
    Shader* _shader;
    // Tablica mieszcz�ca plansz� do gry w k�ko i krzy�yk
    char _board[3][3];
    // Tablica mieszcz�ca pozycje p�l
    glm::vec3 _tile_position[3][3];
    // Wska�niki symbolizuj�ce gracza pierwszego i drugiego
    Player *_player_one, *_player_two;

public:
    // Konstruktor klasy
    Board(GLFWwindow* window);
    // Destruktor klasy
    ~Board();
    // Zresetuj stan planszy
    void Reset();
    // Wywo�aj nast�pn� tur�
    void NextTurn();
    // Sprawd� czy plansza zosta�a w pe�ni zape�niona
    bool IsMovesLeft();
    // Narysuj stan planszy w oknie kontekstowym
    void PrintBoard();
    // Rozpocznij gr�
    void Play();
    // Wypisz wynik starcia w konsoli
    void CongratsWinner();

private:
    // Przechwytuj eventy zwi�zane z klawiatur�
    void _ProcessInput();
    // Stw�rz graczy w oparciu o zapytanie konsolowe
    void _CreatePlayers();

};

