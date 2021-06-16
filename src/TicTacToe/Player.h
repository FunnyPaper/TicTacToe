#pragma once
#include "../Texture.h"
#include "GLFW\glfw3.h"

class Player
{
    // Klasa Gracza
protected:
    // Jakim znakiem operuje gracz (O/X)
    char _sign;
    // Czy obecne padane podejœcie to maximizer czy minimizer + czy to moja kolej na wykonanie ruchu
    bool _isMaximizer, _myTurn;
    // Graficzna reprezentacja znaku którym pos³uguje siê gracz
    Texture* _sign_graphic;
public:
    // Konstruktor
    Player(char sign);
    // Wirtualny destruktor -> wymagany w przypadku dziedziczenia
    virtual ~Player() { delete _sign_graphic; }
    // Wykonaj ruch -> cz³owiek wykona ruch zupe³nie inaczej ni¿ komputer (dlatego jest to metoda czysto wirtualna)
    virtual bool MakeMove(GLFWwindow* window, char _board[3][3]) = 0;
    // Zwróæ znak którym pos³uguje siê gracz
    char GetSign() const { return _sign; }
    // SprawdŸ czy obecne przeszukanie to maximizer
    bool IsMaximizer() const { return _isMaximizer; }
    // Aktywuj teksturê przypisan¹ do gracza
    void UseTexture() const { _sign_graphic->Bind(0); }
    // Zmieñ to czy gracz powinien wykonaæ ruch (true/false)
    void SwapPriority() { _myTurn ^= 1; }
    // SprawdŸ czy ten gracz powinien wykonaæ ruch
    bool GetPriority() { return _myTurn; }
    // Zresetuj gracza (Do stanu z pocz¹tku -> czy powinien wykonaæ ruch)
    void Reset() { _myTurn = (_sign == 'X'); }
    // Policz zawartoœæ planszy (Ka¿dy element planszy posiada wartoœæ -> na podstawie ich sumy mo¿na okreœliæ kto wygra³)
    static int _EvaluateBoard(char _board[3][3]);
protected:
    // SprawdŸ czy plansza nie jest zape³niona
    bool _IsMovesLeft(char _board[3][3]);
    // Algorytm MiniMa -> zwróæ optymalny (bezb³êdny) ruch
    int _MiniMax(char board[3][3], unsigned int depth, bool isMax);
};

class Computer : public Player
{
    // Klasa komputera
public:
    // Konstruktor
    // Wywo³aj kontruktor klasy nadrzêdnej
    Computer(char sign) : Player(sign) {}
    // Destruktor
    ~Computer() {}
    // Definicja dla wykonywania ruchu
    bool MakeMove(GLFWwindow* window, char _board[3][3]);
};

class Human : public Player
{
public:
    // Klasa cz³owieka
    // Wywo³aj konstruktor klasy nadrzêdnej
    Human(char sign) : Player(sign) {}
    // Destruktor
    ~Human() {}
    // Definicja dla wykonania ruchu
    bool MakeMove(GLFWwindow* window, char _board[3][3]);
};