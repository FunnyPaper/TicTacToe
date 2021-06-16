#include "Player.h"
#include <algorithm>
#include <iostream>
#include <GLFW\glfw3.h>
#include "..\Camera.h"

// Do przeliczania w której czêœci ekranu znajduje siê myszka
const unsigned int WIDTH = CAMERA_DEFAULT::WIDTH, HEIGHT = CAMERA_DEFAULT::HEIGHT;

Player::Player(char sign) 
    : _sign(sign) 
{ 
    // Konstruktor
    // Na podstawie podanego znaku stwierdŸ czy ten gracz maksyamlizuje swój wynik
    _isMaximizer = (sign == 'X');
    // Na podstawie podanego znaku wczytaj odpowiedni¹ teksturê
    _sign_graphic = (sign == 'X' ?
        new Texture("res/textures/krzyzyk.png") :
        new Texture("res/textures/kolko.png"));
    // Na podstawie podanego znaku stwierdŸ czy ten gracz zaczyna
    _myTurn = (sign == 'X');
}
bool Player::_IsMovesLeft(char _board[3][3])
{
    // SprawdŸ czy plansza jest zape³niona
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (_board[i][j] == '-') return true;
    return false;
}
int Player::_EvaluateBoard(char _board[3][3])
{
    // SprawdŸ kto wygra³ w tej rundzie (10, -10, 0)
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
    // Algorytm licz¹cy najbardziej optymalny ruch
    // Dla danego pola wyliczana jest wartoœæ oznaczaj¹ca jak bardzo ten ruch przyczyni siê do zwyciêstwa
    int score = _EvaluateBoard(board);

    if (score == 10)
        return score - depth;

    if (score == -10)
        return score + depth;

    if (!_IsMovesLeft(board))
        return 0;

    // Jeœli gracz maksymalizuje swój wynik (do 10)
    if (isMax)
    {
        // Ustaw dostatecznie nisk¹ wartoœæ pocz¹tkow¹
        int best = -1000;

        // W pêtli znajdŸ pierwsze puste miejsce i wype³nij je 'X' (z za³o¿enia X zawsze bêdzie maksymalizowa³)
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    // Wywo³aj raz jeszcze ten sam algorytm po to by dowiedzieæ siê czy wybranie innego pola ma wiêkszy sens po czym cofnij operacjê
                    // i zapisz wartoœæ pola
                    board[i][j] = 'X';
                    best = std::max(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        // Zwróæ wartoœæ pola (odejmujemy g³êbiê ¿eby zaznaczyæ, ¿e nawet jeœli dane pole zwróci³oby 10 
        // to jest to krok w przysz³oœæ wiêc powinno mieæ mniejsz¹ wartoœæ)
        return best - depth;
    }
    // Jeœli gracz minimalizuje swój wynik (do -10)
    else
    {
        // Ustaw dostatecznie wysok¹ wartoœæ pocz¹tkow¹
        int best = 1000;

        // W pêtli znajdŸ pierwsze puste miejsce i wype³nij je 'O' (z za³o¿enia O zawsze bêdzie minimalizowa³)
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    // Wywo³aj raz jeszcze ten sam algorytm po to by dowiedzieæ siê czy wybranie innego pola ma wiêkszy sens po czym cofnij operacjê
                    // i zapisz wartoœæ pola
                    board[i][j] = 'O';
                    best = std::min(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        // Zwróæ wartoœæ pola (dodajemy g³êbiê ¿eby zaznaczyæ, ¿e nawet jeœli dane pole zwróci³oby -10 
        // to jest to krok w przysz³oœæ wiêc powinno mieæ wiêksz¹ wartoœæ)
        return best + depth;
    }
}
bool Computer::MakeMove(GLFWwindow* window, char _board[3][3])
{
    // Komputer wykonuje swój ruch za pomoc¹ tego algorytmu
    int bestVal = 0;
    unsigned int this_row = 0, this_col = 0;

    // Sprawdzamy znak jakim pos³uguje siê komputer
    if (_sign == 'X')
    {
        // Ustaw dostatecznie nisk¹ wartoœæ pocz¹tkow¹
        bestVal = -1000;
        // Wywo³uj w pêtli algorytm MiniMax dla ka¿dego pustego pola by uzyskaæ jego wartoœæ
        // Ta czêœæ wygl¹da bardzo podobnie do samego MiniMax
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    // Wype³nij puste pole
                    _board[i][j] = _sign;

                    // W nastêpnej turze wystêpuje 'O' (nie maksymalizujemy)
                    int best = _MiniMax(_board, 0, 0);

                    // SprawdŸ czy uzyskana z pola wartoœæ jest wiêksza, a jeœli tak to zapisz pozycje tego pola do zmiennych oraz uzyskan¹ wartoœæ
                    if (best > bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }
                    
                    // Cofnij wype³nienie
                    _board[i][j] = '-';
                }
            }
        }
    }
    else if (_sign == 'O')
    {
        // Ustaw dostatecznie wysok¹ wartoœæ pocz¹tkow¹
        bestVal = 1000;
        // Wywo³uj w pêtli algorytm MiniMax dla ka¿dego pustego pola by uzyskaæ jego wartoœæ
        // Ta czêœæ wygl¹da bardzo podobnie do samego MiniMax
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    // Wype³nij puste pole
                    _board[i][j] = _sign;

                    // W nastêpnej turze wystêpuje 'X' (maksymalizujemy)
                    int best = _MiniMax(_board, 0, 1);

                    // SprawdŸ czy uzyskana z pola wartoœæ jest mniejszaa, a jeœli tak to zapisz pozycje tego pola do zmiennych oraz uzyskan¹ wartoœæ
                    if (best < bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }

                    // Cofnij wype³nianie
                    _board[i][j] = '-';
                }
            }
        }
    }
    // Wype³nij pole które posiada najlepsz¹ wartoœæ znakiem u¿ywanym przez komputer
    _board[this_row][this_col] = _sign;
    return true;
}
bool Human::MakeMove(GLFWwindow* window, char _board[3][3])
{
    // Cz³owiek wykonuje ruch za pomoc¹ tego algorytmu
    // Jeœli wykryto klikniêcie mysz¹ wewn¹trz okna kontekstowego
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        
        // Gdzie dok³adnie klikniêto (uzyskaj wspó³rzêdne x i y)
        glfwGetCursorPos(window, &xpos, &ypos);

        // Wartoœci logiczne mówi¹ce o tym czy klikniêto w dane pole
        bool left_up =          xpos < WIDTH / 3 &&     ypos < HEIGHT / 3 &&     xpos > 0 &&             ypos > 0;
        bool middle_up =        xpos < WIDTH * 2 / 3 && ypos < HEIGHT / 3 &&     xpos > WIDTH / 3 &&     ypos > 0;
        bool right_up =         xpos < WIDTH &&         ypos < HEIGHT / 3 &&     xpos > WIDTH * 2 / 3 && ypos > 0;
        bool left_middle =      xpos < WIDTH / 3 &&     ypos < HEIGHT * 2 / 3 && xpos > 0 &&             ypos > HEIGHT / 3;
        bool middle_middle =    xpos < WIDTH * 2 / 3 && ypos < HEIGHT * 2 / 3 && xpos > WIDTH / 3 &&     ypos > HEIGHT / 3;
        bool right_middle =     xpos < WIDTH &&         ypos < HEIGHT * 2 / 3 && xpos > WIDTH * 2 / 3 && ypos > HEIGHT / 3;
        bool left_down =        xpos < WIDTH / 3 &&     ypos < HEIGHT &&         xpos > 0 &&             ypos > HEIGHT * 2 / 3;
        bool middle_down =      xpos < WIDTH * 2 / 3 && ypos < HEIGHT &&         xpos > WIDTH / 3 &&     ypos > HEIGHT * 2 / 3;
        bool right_down =       xpos < WIDTH &&         ypos < HEIGHT &&         xpos > WIDTH * 2 / 3 && ypos > HEIGHT * 2 / 3;
        
        // SprawdŸ czy wybrane pole jest puste, a jeœli tak to je wype³nij znakiem
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