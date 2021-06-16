#pragma once
#include "../Texture.h"
#include "GLFW\glfw3.h"

class Player
{
    // Klasa Gracza
protected:
    // Jakim znakiem operuje gracz (O/X)
    char _sign;
    // Czy obecne padane podej�cie to maximizer czy minimizer + czy to moja kolej na wykonanie ruchu
    bool _isMaximizer, _myTurn;
    // Graficzna reprezentacja znaku kt�rym pos�uguje si� gracz
    Texture* _sign_graphic;
public:
    // Konstruktor
    Player(char sign);
    // Wirtualny destruktor -> wymagany w przypadku dziedziczenia
    virtual ~Player() { delete _sign_graphic; }
    // Wykonaj ruch -> cz�owiek wykona ruch zupe�nie inaczej ni� komputer (dlatego jest to metoda czysto wirtualna)
    virtual bool MakeMove(GLFWwindow* window, char _board[3][3]) = 0;
    // Zwr�� znak kt�rym pos�uguje si� gracz
    char GetSign() const { return _sign; }
    // Sprawd� czy obecne przeszukanie to maximizer
    bool IsMaximizer() const { return _isMaximizer; }
    // Aktywuj tekstur� przypisan� do gracza
    void UseTexture() const { _sign_graphic->Bind(0); }
    // Zmie� to czy gracz powinien wykona� ruch (true/false)
    void SwapPriority() { _myTurn ^= 1; }
    // Sprawd� czy ten gracz powinien wykona� ruch
    bool GetPriority() { return _myTurn; }
    // Zresetuj gracza (Do stanu z pocz�tku -> czy powinien wykona� ruch)
    void Reset() { _myTurn = (_sign == 'X'); }
    // Policz zawarto�� planszy (Ka�dy element planszy posiada warto�� -> na podstawie ich sumy mo�na okre�li� kto wygra�)
    static int _EvaluateBoard(char _board[3][3]);
protected:
    // Sprawd� czy plansza nie jest zape�niona
    bool _IsMovesLeft(char _board[3][3]);
    // Algorytm MiniMa -> zwr�� optymalny (bezb��dny) ruch
    int _MiniMax(char board[3][3], unsigned int depth, bool isMax);
};

class Computer : public Player
{
    // Klasa komputera
public:
    // Konstruktor
    // Wywo�aj kontruktor klasy nadrz�dnej
    Computer(char sign) : Player(sign) {}
    // Destruktor
    ~Computer() {}
    // Definicja dla wykonywania ruchu
    bool MakeMove(GLFWwindow* window, char _board[3][3]);
};

class Human : public Player
{
public:
    // Klasa cz�owieka
    // Wywo�aj konstruktor klasy nadrz�dnej
    Human(char sign) : Player(sign) {}
    // Destruktor
    ~Human() {}
    // Definicja dla wykonania ruchu
    bool MakeMove(GLFWwindow* window, char _board[3][3]);
};