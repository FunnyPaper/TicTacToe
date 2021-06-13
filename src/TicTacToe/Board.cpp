#include "Board.h"
#include <iostream>

Board::Board(GLFWwindow* window, PLAYERS player_one, PLAYERS player_two) :  _window(window)
{
    // Jakis randomizer trzeba dodac
    if(player_one == PLAYERS::COMPUTER) _player_one = new Computer('X');
    else if(player_one == PLAYERS::HUMAN) _player_one = new Human('X');

    if(player_two == PLAYERS::COMPUTER) _player_two = new Computer('O');
    else if(player_two == PLAYERS::HUMAN) _player_two = new Human('O');

    _tile = new Mesh(new Quad(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(1.7f));
    _tile_position[0][0] = glm::vec3(-1.7f, 1.7f, 0.0f);
    _tile_position[0][1] = glm::vec3(0.0f, 1.7f, 0.0f),
    _tile_position[0][2] = glm::vec3(1.7f, 1.7f, 0.0f),
    _tile_position[1][0] = glm::vec3(-1.7f, 0.0f, 0.0f),
    _tile_position[1][1] = glm::vec3(0.0f, 0.0f, 0.0f),
    _tile_position[1][2] = glm::vec3(1.7f, 0.0f, 0.0f),
    _tile_position[2][0] = glm::vec3(-1.7f, -1.7f, 0.0f),
    _tile_position[2][1] = glm::vec3(0.0f, -1.7f, 0.0f),
    _tile_position[2][2] = glm::vec3(1.7f, -1.7f, 0.0f),
    Reset();
}
Board::~Board()
{
    delete _player_one; delete _player_two;
}
void Board::Reset()
{
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            _board[i][j] = '-';

    _player_one->Reset(); _player_two->Reset();
}
void Board::NextTurn()
{
    _player_one->SwapPriority(); _player_two->SwapPriority();
}
bool Board::IsMovesLeft()
{
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (_board[i][j] == '-') return true;
    return false;
}
void Board::PrintBoard(Shader& shader)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            if (_board[i][j] == 'X')
            {
                shader.SetUniform1i("is_texture_attached", 1);
                _player_one->UseTexture();
            }
            else if (_board[i][j] == 'O')
            {
                shader.SetUniform1i("is_texture_attached", 1);
                _player_two->UseTexture();
            }
            else
            {
                shader.SetUniform1i("is_texture_attached", 0);
                Texture::Unbind();
            }
            _tile->SetPosition(_tile_position[i][j]);
            _tile->Render(&shader);
        }
    }
}
void Board::Play(Shader& shader)
{
    PrintBoard(shader);
    
    if (!IsMovesLeft() || Player::_EvaluateBoard(_board) != 0)
    {
        CongratsWinner();
        Reset();
    }
    else if (IsMovesLeft() && _player_one->GetPriority())
    {
        if(_player_one->MakeMove(_window, _board)) NextTurn();
    }
    else if (IsMovesLeft() && _player_two->GetPriority())
    {
        if(_player_two->MakeMove(_window, _board)) NextTurn();
    }
}
void Board::CongratsWinner()
{
    int score = Player::_EvaluateBoard(_board);
    std::cout << (score == 10 ? "Player one has won!" : score == -10 ? "Player two has one!" : "Tie") << std::endl;
}