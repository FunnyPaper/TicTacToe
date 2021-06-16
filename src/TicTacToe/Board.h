#pragma once
#include "Player.h"
#include "../Mesh.h"
#include <glm\glm.hpp>
#include "../Shader.h"
#include "../Camera.h"
#include <GLFW\glfw3.h>

class Board
{
    // Klasa odpowiedzialna za rysowanie planszy i proces gry (który gracz powinien wykonaæ ruch)
private:
    // WskaŸnik na okno kontekstowe
    GLFWwindow* _window;
    // WskaŸnik na pojedyncze pole
    Mesh *_tile;
    // WskaŸnik na shader stosowany do renderowania grafiki
    Shader* _shader;
    // Tablica mieszcz¹ca planszê do gry w kó³ko i krzy¿yk
    char _board[3][3];
    // Tablica mieszcz¹ca pozycje pól
    glm::vec3 _tile_position[3][3];
    // WskaŸniki symbolizuj¹ce gracza pierwszego i drugiego
    Player *_player_one, *_player_two;

public:
    // Konstruktor klasy
    Board(GLFWwindow* window);
    // Destruktor klasy
    ~Board();
    // Zresetuj stan planszy
    void Reset();
    // Wywo³aj nastêpn¹ turê
    void NextTurn();
    // SprawdŸ czy plansza zosta³a w pe³ni zape³niona
    bool IsMovesLeft();
    // Narysuj stan planszy w oknie kontekstowym
    void PrintBoard();
    // Rozpocznij grê
    void Play();
    // Wypisz wynik starcia w konsoli
    void CongratsWinner();

private:
    // Przechwytuj eventy zwi¹zane z klawiatur¹
    void _ProcessInput();
    // Stwórz graczy w oparciu o zapytanie konsolowe
    void _CreatePlayers();

};

