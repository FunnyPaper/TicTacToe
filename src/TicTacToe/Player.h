#pragma once
#include "../Texture.h"
#include "GLFW\glfw3.h"

class Player
{
protected:
    char _sign;
    bool _isMaximizer, _myTurn;
    Texture* _sign_graphic;
public:
    Player(char sign);
    virtual ~Player() { delete _sign_graphic; }
    virtual bool MakeMove(GLFWwindow* window, char _board[3][3]) = 0;
    char GetSign() const { return _sign; }
    bool IsMaximizer() const { return _isMaximizer; }
    void UseTexture() const { _sign_graphic->Bind(0); }
    void SwapPriority() { _myTurn ^= 1; }
    bool GetPriority() { return _myTurn; }
    void Reset() { _myTurn = (_sign == 'X'); }
    static int _EvaluateBoard(char _board[3][3]);
protected:
    bool _IsMovesLeft(char _board[3][3]);
    int _MiniMax(char board[3][3], unsigned int depth, bool isMax);
};

class Computer : public Player
{
public:
    Computer(char sign) : Player(sign) {}
    ~Computer() {}
    bool MakeMove(GLFWwindow* window, char _board[3][3]);
};

class Human : public Player
{
public:
    Human(char sign) : Player(sign) {}
    ~Human() {}
    bool MakeMove(GLFWwindow* window, char _board[3][3]);
};