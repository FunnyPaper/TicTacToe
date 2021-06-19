#include "Player.h"
#include <algorithm>
#include <iostream>
#include <GLFW\glfw3.h>
#include "..\Camera.h"

// Do przeliczania w której części ekranu znajduje się myszka
const unsigned int WIDTH = CAMERA_DEFAULT::WIDTH, HEIGHT = CAMERA_DEFAULT::HEIGHT;

Player::Player(char sign) 
    : _sign(sign) 
{ 
    // Konstruktor
    // Na podstawie podanego znaku stwierdź czy ten gracz maksyamlizuje swój wynik
    _isMaximizer = (sign == 'X');
    // Na podstawie podanego znaku wczytaj odpowiednią teksturę
    _sign_graphic = (sign == 'X' ?
        new Texture("res/textures/krzyzyk.png") :
        new Texture("res/textures/kolko.png"));
    // Na podstawie podanego znaku stwierdź czy ten gracz zaczyna
    _myTurn = (sign == 'X');
}
bool Player::_IsMovesLeft(char _board[3][3])
{
    // Sprawdź czy plansza jest zapełniona
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (_board[i][j] == '-') return true;
    return false;
}
int Player::_EvaluateBoard(char _board[3][3])
{
    // Sprawdź kto wygrał w tej rundzie (10, -10, 0)
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
    // Algorytm liczący najbardziej optymalny ruch
    // Dla danego pola wyliczana jest wartość oznaczająca jak bardzo ten ruch przyczyni się do zwycięstwa
    int score = _EvaluateBoard(board);

    if (score == 10)
        return score - depth;

    if (score == -10)
        return score + depth;

    if (!_IsMovesLeft(board))
        return 0;

    // Jeśli gracz maksymalizuje swój wynik (do 10)
    if (isMax)
    {
        // Ustaw dostatecznie niską wartość początkową
        int best = -1000;

        // W pętli znajdź pierwsze puste miejsce i wypełnij je 'X' (z założenia X zawsze będzie maksymalizował)
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    // Wywołaj raz jeszcze ten sam algorytm po to by dowiedzieć się czy wybranie innego pola ma większy sens po czym cofnij operację
                    // i zapisz wartość pola
                    board[i][j] = 'X';
                    best = std::max(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        // Zwróć wartość pola (odejmujemy głębię żeby zaznaczyć, że nawet jeśli dane pole zwróciłoby 10 
        // to jest to krok w przyszłość więc powinno mieć mniejszą wartość)
        return best - depth;
    }
    // Jeśli gracz minimalizuje swój wynik (do -10)
    else
    {
        // Ustaw dostatecznie wysoką wartość początkową
        int best = 1000;

        // W pętli znajdź pierwsze puste miejsce i wypełnij je 'O' (z założenia O zawsze będzie minimalizował)
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')
                {
                    // Wywołaj raz jeszcze ten sam algorytm po to by dowiedzieć się czy wybranie innego pola ma większy sens po czym cofnij operację
                    // i zapisz wartość pola
                    board[i][j] = 'O';
                    best = std::min(_MiniMax(board, depth + 1, !isMax), best);
                    board[i][j] = '-';
                }
            }
        }
        // Zwróć wartość pola (dodajemy głębię żeby zaznaczyć, że nawet jeśli dane pole zwróciłoby -10 
        // to jest to krok w przyszłość więc powinno mieć większą wartość)
        return best + depth;
    }
}
bool Computer::MakeMove(GLFWwindow* window, char _board[3][3])
{
    // Komputer wykonuje swój ruch za pomocą tego algorytmu
    int bestVal = 0;
    unsigned int this_row = 0, this_col = 0;

    // Sprawdzamy znak jakim posługuje się komputer
    if (_sign == 'X')
    {
        // Ustaw dostatecznie niską wartość początkową
        bestVal = -1000;
        // Wywołuj w pętli algorytm MiniMax dla każdego pustego pola by uzyskać jego wartość
        // Ta część wygląda bardzo podobnie do samego MiniMax
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    // Wypełnij puste pole
                    _board[i][j] = _sign;

                    // W następnej turze występuje 'O' (nie maksymalizujemy)
                    int best = _MiniMax(_board, 0, 0);

                    // Sprawdź czy uzyskana z pola wartość jest większa, a jeśli tak to zapisz pozycje tego pola do zmiennych oraz uzyskaną wartość
                    if (best > bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }
                    
                    // Cofnij wypełnienie
                    _board[i][j] = '-';
                }
            }
        }
    }
    else if (_sign == 'O')
    {
        // Ustaw dostatecznie wysoką wartość początkową
        bestVal = 1000;
        // Wywołuj w pętli algorytm MiniMax dla każdego pustego pola by uzyskać jego wartość
        // Ta część wygląda bardzo podobnie do samego MiniMax
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                if (_board[i][j] == '-')
                {
                    // Wypełnij puste pole
                    _board[i][j] = _sign;

                    // W następnej turze występuje 'X' (maksymalizujemy)
                    int best = _MiniMax(_board, 0, 1);

                    // Sprawdź czy uzyskana z pola wartość jest mniejszaa, a jeśli tak to zapisz pozycje tego pola do zmiennych oraz uzyskaną wartość
                    if (best < bestVal)
                    {
                        this_row = i; this_col = j;
                        bestVal = best;
                    }

                    // Cofnij wypełnianie
                    _board[i][j] = '-';
                }
            }
        }
    }
    // Wypełnij pole które posiada najlepszą wartość znakiem używanym przez komputer
    _board[this_row][this_col] = _sign;
    return true;
}
bool Human::MakeMove(GLFWwindow* window, char _board[3][3])
{
    // Człowiek wykonuje ruch za pomocą tego algorytmu
    // Jeśli wykryto kliknięcie myszą wewnątrz okna kontekstowego
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        
        // Gdzie dokładnie kliknięto (uzyskaj współrzędne x i y)
        glfwGetCursorPos(window, &xpos, &ypos);

        // Wartości logiczne mówiące o tym czy kliknięto w dane pole
        bool left_up =          xpos < WIDTH / 3 &&     ypos < HEIGHT / 3 &&     xpos > 0 &&             ypos > 0;
        bool middle_up =        xpos < WIDTH * 2 / 3 && ypos < HEIGHT / 3 &&     xpos > WIDTH / 3 &&     ypos > 0;
        bool right_up =         xpos < WIDTH &&         ypos < HEIGHT / 3 &&     xpos > WIDTH * 2 / 3 && ypos > 0;
        bool left_middle =      xpos < WIDTH / 3 &&     ypos < HEIGHT * 2 / 3 && xpos > 0 &&             ypos > HEIGHT / 3;
        bool middle_middle =    xpos < WIDTH * 2 / 3 && ypos < HEIGHT * 2 / 3 && xpos > WIDTH / 3 &&     ypos > HEIGHT / 3;
        bool right_middle =     xpos < WIDTH &&         ypos < HEIGHT * 2 / 3 && xpos > WIDTH * 2 / 3 && ypos > HEIGHT / 3;
        bool left_down =        xpos < WIDTH / 3 &&     ypos < HEIGHT &&         xpos > 0 &&             ypos > HEIGHT * 2 / 3;
        bool middle_down =      xpos < WIDTH * 2 / 3 && ypos < HEIGHT &&         xpos > WIDTH / 3 &&     ypos > HEIGHT * 2 / 3;
        bool right_down =       xpos < WIDTH &&         ypos < HEIGHT &&         xpos > WIDTH * 2 / 3 && ypos > HEIGHT * 2 / 3;
        
        // Sprawdź czy wybrane pole jest puste, a jeśli tak to je wypełnij znakiem
        if (left_up && _board[0][0] == '-') { _board[0][0] = _sign; return true; }
        else if (middle_up && _board[0][1] == '-') { _board[0][1] = _sign; return true; }
        else if (right_up && _board[0][2] == '-') { _board[0][2] = _sign; return true; }
        else if (left_middle && _board[1][0] == '-') { _board[1][0] = _sign; return true; }
        else if (middle_middle && _board[1][1] == '-') { _board[1][1] = _sign; return true; }
        else if (right_middle && _board[1][2] == '-') { _board[1][2] = _sign; return true; }
        else if (left_down && _board[2][0] == '-') { _board[2][0] = _sign; return true; }
        else if (middle_down && _board[2][1] == '-') { _board[2][1] = _sign; return true; }
        else if (right_down && _board[2][2] == '-') { _board[2][2] = _sign; return true; }
    }
    return false;
}
