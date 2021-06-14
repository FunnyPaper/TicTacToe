#pragma once
#include "Player.h"
#include "../Mesh.h"
#include <glm\glm.hpp>
#include "../Shader.h"
#include "../Camera.h"
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
    Shader* _shader;
    Camera* _camera;
    char _board[3][3];
    unsigned int _x, _y;
    glm::vec3 _tile_position[3][3];
    Player *_player_one, *_player_two;
public:
    Board(GLFWwindow* window, PLAYERS player_one, PLAYERS player_two);
    ~Board();
    void Reset();
    void NextTurn();
    bool IsMovesLeft();
    void PrintBoard();
    void Play();
    void CongratsWinner();
};

