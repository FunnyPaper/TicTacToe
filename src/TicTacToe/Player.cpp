#include "Player.h"
#include <algorithm>
#include <iostream>
#include <GLFW\glfw3.h>

const unsigned int WIDTH = 500, HEIGHT = WIDTH;

Player::Player(char sign) 
    : _sign(sign) 
{ 
    _isMaximizer = (sign == 'X');
    _sign_graphic = (sign == 'X' ?
        new Texture("res/textures/krzyzyk.png") :
        new Texture("res/textures/kolko.png"));
    _myTurn = (sign == 'X');
}
bool Player::_IsMovesLeft(char _board[3][3])
{
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (_board[i][j] == '-') return true;
    return false;
}
int Player::_EvaluateBoard(char _board[3][3])
{
    // Poziomo
    for (unsigned int i = 0; i < 3; i++)
    {
        if (_board[i][0] == _board[i][1] && _board[i][1] == _board[i][2])
        {
            if (_board[i][0] == 'X') return 10;
            else if (_board[i][0] == 'O') return -10;
        }
    }
    // Pionowo
    for (unsigned int i = 0; i < 3; i++)
    {
        if (_board[0][i] == _board[1][i] && _board[1][i] == _board[2][i])
        {
            if (_board[0][i] == 'X') return 10;
            else if (_board[0][i] == 'O') return -10;
        }
    }
    // Po przekatnej
    {
        if (_board[0][2] == _board[1][1] && _board[1][1] == _board[2][0])
        {
            if (_board[1][1] == 'X') return 10;
            else if (_board[1][1] == 'O') return -10;
        }
        if (_board[0][0] == _board[1][1] && _board[1][1] == _board[2][2])
        {
            if (_board[0][0] == 'X') return 10;
            else if (_board[0][0] == 'O') return -10;
        }
    }
    return 0;
}
int Player::_MiniMax(char board[3][3], unsigned int depth, bool isMax)
{
    int score = _EvaluateBoard(board);

    if (score == 10)
        return score - depth;

    if (score == -10)
        return score + depth;

    if (!_IsMovesLeft(board))
        return 0;

    if (isMax)
    {
        int best = -1000;

        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    board[i][j] = 'X';
                    best = std::max(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        return best - depth;
    }
    else
    {
        int best = 1000;

        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    board[i][j] = 'O';
                    best = std::min(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        return best + depth;
    }
}
bool Computer::MakeMove(GLFWwindow* window, char _board[3][3])
{
    int bestVal = 0;
    unsigned int this_row = 0, this_col = 0;

    if (_sign == 'X')
    {
        bestVal = -1000;
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    _board[i][j] = _sign;

                    int best = _MiniMax(_board, 0, 0); // O is next turn
                    //printBoard(board);

                    if (best > bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }
                    _board[i][j] = '-';
                }
            }
        }
    }
    else if (_sign == 'O')
    {
        bestVal = 1000;
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    _board[i][j] = _sign;

                    int best = _MiniMax(_board, 0, 1); // O is next turn
                    //printBoard(board);

                    if (best < bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }
                    _board[i][j] = '-';
                }
            }
        }
    }
    _board[this_row][this_col] = _sign;
    return true;
}
bool Human::MakeMove(GLFWwindow* window, char _board[3][3])
{
    /*{
        unsigned int a, b;
        std::cout << "Twoj ruch[W przypadku zajetych pol opcja wyboru pojawi sie raz jeszcze]:\n";
        do
        {
            std::cout << "wiersz[1-3]: "; std::cin >> a;
            std::cout << "kolumna[1-3]: "; std::cin >> b;
        } while (_board[a - 1][b - 1] != '-');
        _board[a - 1][b - 1] = _sign;
    }*/

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        bool left_up =          xpos < WIDTH / 3 &&     ypos < HEIGHT / 3 &&     xpos > 0 &&             ypos > 0;
        bool middle_up =        xpos < WIDTH * 2 / 3 && ypos < HEIGHT / 3 &&     xpos > WIDTH / 3 &&     ypos > 0;
        bool right_up =         xpos < WIDTH &&         ypos < HEIGHT / 3 &&     xpos > WIDTH * 2 / 3 && ypos > 0;
        bool left_middle =      xpos < WIDTH / 3 &&     ypos < HEIGHT * 2 / 3 && xpos > 0 &&             ypos > HEIGHT / 3;
        bool middle_middle =    xpos < WIDTH * 2 / 3 && ypos < HEIGHT * 2 / 3 && xpos > WIDTH / 3 &&     ypos > HEIGHT / 3;
        bool right_middle =     xpos < WIDTH &&         ypos < HEIGHT * 2 / 3 && xpos > WIDTH * 2 / 3 && ypos > HEIGHT / 3;
        bool left_down =        xpos < WIDTH / 3 &&     ypos < HEIGHT &&         xpos > 0 &&             ypos > HEIGHT * 2 / 3;
        bool middle_down =      xpos < WIDTH * 2 / 3 && ypos < HEIGHT &&         xpos > WIDTH / 3 &&     ypos > HEIGHT * 2 / 3;
        bool right_down =       xpos < WIDTH &&         ypos < HEIGHT &&         xpos > WIDTH * 2 / 3 && ypos > HEIGHT * 2 / 3;
        // Jakos to trzeba przeksztalcic
        // xpos -= WIDTH / 2; ypos -= HEIGHT / 2; ypos = -ypos;
        if (left_up && _board[0][0] == '-') { _board[0][0] = _sign; return true; }
        else if (middle_up && _board[0][1] == '-') { _board[0][1] = _sign; return true; }
        else if (right_up && _board[0][2] == '-') { _board[0][2] = _sign; return true; }
        else if (left_middle && _board[1][0] == '-') { _board[1][0] = _sign; return true; }
        else if (middle_middle && _board[1][1] == '-') { _board[1][1] = _sign; return true; }
        else if (right_middle && _board[1][2] == '-') { _board[1][2] = _sign; return true; }
        else if (left_down && _board[2][0] == '-') { _board[2][0] = _sign; return true; }
        else if (middle_down && _board[2][1] == '-') { _board[2][1] = _sign; return true; }
        else if (right_down && _board[2][2] == '-') { _board[2][2] = _sign; return true; }
        return false;
    }
}