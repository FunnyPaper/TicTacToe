#include "Player.h"
#include <algorithm>
#include <iostream>
#include <GLFW\glfw3.h>
#include "..\Camera.h"

// Do przeliczania w kt�rej cz�ci ekranu znajduje si� myszka
const unsigned int WIDTH = CAMERA_DEFAULT::WIDTH, HEIGHT = CAMERA_DEFAULT::HEIGHT;

Player::Player(char sign) 
    : _sign(sign) 
{ 
    // Konstruktor
    // Na podstawie podanego znaku stwierd� czy ten gracz maksyamlizuje sw�j wynik
    _isMaximizer = (sign == 'X');
    // Na podstawie podanego znaku wczytaj odpowiedni� tekstur�
    _sign_graphic = (sign == 'X' ?
        new Texture("res/textures/krzyzyk.png") :
        new Texture("res/textures/kolko.png"));
    // Na podstawie podanego znaku stwierd� czy ten gracz zaczyna
    _myTurn = (sign == 'X');
}
bool Player::_IsMovesLeft(char _board[3][3])
{
    // Sprawd� czy plansza jest zape�niona
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (_board[i][j] == '-') return true;
    return false;
}
int Player::_EvaluateBoard(char _board[3][3])
{
    // Sprawd� kto wygra� w tej rundzie (10, -10, 0)
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
    // Algorytm licz�cy najbardziej optymalny ruch
    // Dla danego pola wyliczana jest warto�� oznaczaj�ca jak bardzo ten ruch przyczyni si� do zwyci�stwa
    int score = _EvaluateBoard(board);

    if (score == 10)
        return score - depth;

    if (score == -10)
        return score + depth;

    if (!_IsMovesLeft(board))
        return 0;

    // Je�li gracz maksymalizuje sw�j wynik (do 10)
    if (isMax)
    {
        // Ustaw dostatecznie nisk� warto�� pocz�tkow�
        int best = -1000;

        // W p�tli znajd� pierwsze puste miejsce i wype�nij je 'X' (z za�o�enia X zawsze b�dzie maksymalizowa�)
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    // Wywo�aj raz jeszcze ten sam algorytm po to by dowiedzie� si� czy wybranie innego pola ma wi�kszy sens po czym cofnij operacj�
                    // i zapisz warto�� pola
                    board[i][j] = 'X';
                    best = std::max(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        // Zwr�� warto�� pola (odejmujemy g��bi� �eby zaznaczy�, �e nawet je�li dane pole zwr�ci�oby 10 
        // to jest to krok w przysz�o�� wi�c powinno mie� mniejsz� warto��)
        return best - depth;
    }
    // Je�li gracz minimalizuje sw�j wynik (do -10)
    else
    {
        // Ustaw dostatecznie wysok� warto�� pocz�tkow�
        int best = 1000;

        // W p�tli znajd� pierwsze puste miejsce i wype�nij je 'O' (z za�o�enia O zawsze b�dzie minimalizowa�)
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    // Wywo�aj raz jeszcze ten sam algorytm po to by dowiedzie� si� czy wybranie innego pola ma wi�kszy sens po czym cofnij operacj�
                    // i zapisz warto�� pola
                    board[i][j] = 'O';
                    best = std::min(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        // Zwr�� warto�� pola (dodajemy g��bi� �eby zaznaczy�, �e nawet je�li dane pole zwr�ci�oby -10 
        // to jest to krok w przysz�o�� wi�c powinno mie� wi�ksz� warto��)
        return best + depth;
    }
}
bool Computer::MakeMove(GLFWwindow* window, char _board[3][3])
{
    // Komputer wykonuje sw�j ruch za pomoc� tego algorytmu
    int bestVal = 0;
    unsigned int this_row = 0, this_col = 0;

    // Sprawdzamy znak jakim pos�uguje si� komputer
    if (_sign == 'X')
    {
        // Ustaw dostatecznie nisk� warto�� pocz�tkow�
        bestVal = -1000;
        // Wywo�uj w p�tli algorytm MiniMax dla ka�dego pustego pola by uzyska� jego warto��
        // Ta cz�� wygl�da bardzo podobnie do samego MiniMax
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    // Wype�nij puste pole
                    _board[i][j] = _sign;

                    // W nast�pnej turze wyst�puje 'O' (nie maksymalizujemy)
                    int best = _MiniMax(_board, 0, 0);

                    // Sprawd� czy uzyskana z pola warto�� jest wi�ksza, a je�li tak to zapisz pozycje tego pola do zmiennych oraz uzyskan� warto��
                    if (best > bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }
                    
                    // Cofnij wype�nienie
                    _board[i][j] = '-';
                }
            }
        }
    }
    else if (_sign == 'O')
    {
        // Ustaw dostatecznie wysok� warto�� pocz�tkow�
        bestVal = 1000;
        // Wywo�uj w p�tli algorytm MiniMax dla ka�dego pustego pola by uzyska� jego warto��
        // Ta cz�� wygl�da bardzo podobnie do samego MiniMax
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    // Wype�nij puste pole
                    _board[i][j] = _sign;

                    // W nast�pnej turze wyst�puje 'X' (maksymalizujemy)
                    int best = _MiniMax(_board, 0, 1);

                    // Sprawd� czy uzyskana z pola warto�� jest mniejszaa, a je�li tak to zapisz pozycje tego pola do zmiennych oraz uzyskan� warto��
                    if (best < bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }

                    // Cofnij wype�nianie
                    _board[i][j] = '-';
                }
            }
        }
    }
    // Wype�nij pole kt�re posiada najlepsz� warto�� znakiem u�ywanym przez komputer
    _board[this_row][this_col] = _sign;
    return true;
}
bool Human::MakeMove(GLFWwindow* window, char _board[3][3])
{
    // Cz�owiek wykonuje ruch za pomoc� tego algorytmu
    // Je�li wykryto klikni�cie mysz� wewn�trz okna kontekstowego
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        
        // Gdzie dok�adnie klikni�to (uzyskaj wsp�rz�dne x i y)
        glfwGetCursorPos(window, &xpos, &ypos);

        // Warto�ci logiczne m�wi�ce o tym czy klikni�to w dane pole
        bool left_up =          xpos < WIDTH / 3 &&     ypos < HEIGHT / 3 &&     xpos > 0 &&             ypos > 0;
        bool middle_up =        xpos < WIDTH * 2 / 3 && ypos < HEIGHT / 3 &&     xpos > WIDTH / 3 &&     ypos > 0;
        bool right_up =         xpos < WIDTH &&         ypos < HEIGHT / 3 &&     xpos > WIDTH * 2 / 3 && ypos > 0;
        bool left_middle =      xpos < WIDTH / 3 &&     ypos < HEIGHT * 2 / 3 && xpos > 0 &&             ypos > HEIGHT / 3;
        bool middle_middle =    xpos < WIDTH * 2 / 3 && ypos < HEIGHT * 2 / 3 && xpos > WIDTH / 3 &&     ypos > HEIGHT / 3;
        bool right_middle =     xpos < WIDTH &&         ypos < HEIGHT * 2 / 3 && xpos > WIDTH * 2 / 3 && ypos > HEIGHT / 3;
        bool left_down =        xpos < WIDTH / 3 &&     ypos < HEIGHT &&         xpos > 0 &&             ypos > HEIGHT * 2 / 3;
        bool middle_down =      xpos < WIDTH * 2 / 3 && ypos < HEIGHT &&         xpos > WIDTH / 3 &&     ypos > HEIGHT * 2 / 3;
        bool right_down =       xpos < WIDTH &&         ypos < HEIGHT &&         xpos > WIDTH * 2 / 3 && ypos > HEIGHT * 2 / 3;
        
        // Sprawd� czy wybrane pole jest puste, a je�li tak to je wype�nij znakiem
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