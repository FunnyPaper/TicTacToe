#pragma once
#include "Player.h"
#include "../Mesh.h"
#include "../Primitives.h"
#include <glm\glm.hpp>
#include "../Shader.h"
#include <GLFW\glfw3.h>

enum class PLAYERS
{
    HUMAN, COMPUTER
};

class Board
{
private:
    GLFWwindow* _window;
    Mesh *_tile;
    char _board[3][3];
    glm::vec3 _tile_position[3][3];
    Player *_player_one, *_player_two;
public:
    Board(GLFWwindow* window, PLAYERS player_one, PLAYERS player_two);
    ~Board();
    void Reset();
    void NextTurn();
    bool IsMovesLeft();
    void PrintBoard(Shader& shader);
    void Play(Shader& shader);
    void CongratsWinner();
};

