#include "Board.h"
#include <iostream>

Board::Board(GLFWwindow* window) : _window(window)
{
    // Stwórz shader w oparciu o lokazlizacjê dwóch plików -> vertex i fragment shader
    _shader = new Shader("res/shaders/vs.shader", "res/shaders/fs.shader");
    // Przypisujemy nazwy slotów tekstur do numerycznych wartoœci (tworzenie ³¹czeñ)
    _shader->SetUniform1i("diffuse_texture0", 0);
    _shader->SetUniform1i("specular_texture1", 1);
    // Stwórz tymczasow¹ kamerê i przeœlij ustawienia do shadera
    // Nie potrzebujemy kontroli nad kamer¹ -> wystarczy nam jej bazowe ustawienie po czym mo¿emy o niej zapomnieæ
    Camera(glm::vec3(0.0f, 0.0f, 0.0f)).SendToShader(_shader);
    // Stwórz pojedyncze pole
    // Zmienne do manipulacji skal¹ pola oraz odstêpami
    float _scaling_factor = 1.65f, _spaces = 0.06f;
    _tile = new Mesh(new Quad(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(_scaling_factor));
    // Przypisz rozmieszczenie pól planszy
    _tile_position[0][0] = glm::vec3(-_scaling_factor - _spaces, _scaling_factor + _spaces, 0.0f);
    _tile_position[0][1] = glm::vec3(0.0f, _scaling_factor + _spaces, 0.0f);
    _tile_position[0][2] = glm::vec3(_scaling_factor + _spaces, _scaling_factor + _spaces, 0.0f);
    _tile_position[1][0] = glm::vec3(-_scaling_factor - _spaces, 0.0f, 0.0f);
    _tile_position[1][1] = glm::vec3(0.0f, 0.0f, 0.0f);
    _tile_position[1][2] = glm::vec3(_scaling_factor + _spaces, 0.0f, 0.0f);
    _tile_position[2][0] = glm::vec3(-_scaling_factor - _spaces, -_scaling_factor - _spaces, 0.0f);
    _tile_position[2][1] = glm::vec3(0.0f, -_scaling_factor - _spaces, 0.0f);
    _tile_position[2][2] = glm::vec3(_scaling_factor + _spaces, -_scaling_factor - _spaces, 0.0f);
    // Ustaw stan pocz¹tkowy planszy (wype³nianie tablicy pustymi wartoœciami)
    Reset();
}
Board::~Board()
{
    // Destruktor
    // Usuñ dynamiczne obiekty
    if (_player_one) delete _player_one;
    if (_player_two) delete _player_two;
    if (_tile) delete _tile;
    if (_shader) delete _shader;
}
void Board::Reset()
{
    // Stwórz nowych graczy
    _CreatePlayers();
    // Czyœcimy ekran co klatkê
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Przestaw ustawienia shadera
    _shader->SetUniform1i("is_texture_attached", 0);
    // Dezaktywuj ostatni¹ teksturê
    Texture::Unbind();
    // W pêtli ustaw stan pocz¹tkowy planszy (pusta) oraz narysuj pust¹ planszê w oknie kontekstowym
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
        {
            _board[i][j] = '-';
            _tile->SetPosition(_tile_position[i][j]);
            _tile->Render(_shader);
        }
}
void Board::NextTurn()
{
    // Wywo³aj nastêpn¹ turê
    // Zmieñ to kto powinien wykonaæ ruch
    _player_one->SwapPriority(); _player_two->SwapPriority();
}
bool Board::IsMovesLeft()
{
    // SprawdŸ czy plansza jest pusta
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (_board[i][j] == '-') return true;
    return false;
}
void Board::PrintBoard()
{
    // Narysuj stan planszy w oknie kontekstowym
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // W zale¿noœci od zawartoœci komórki zmieñ ustawienia shadera oraz aktywuj odpowiedni¹ teksturê
            if (_board[i][j] == 'X')
            {
                _shader->SetUniform1i("is_texture_attached", 1);
                _player_one->UseTexture();
            }
            else if (_board[i][j] == 'O')
            {
                _shader->SetUniform1i("is_texture_attached", 1);
                _player_two->UseTexture();
            }
            else
            {
                _shader->SetUniform1i("is_texture_attached", 0);
                Texture::Unbind();
            }
            // Zmieñ pozycjê obiektu oraz go narysuj
            _tile->SetPosition(_tile_position[i][j]);
            _tile->Render(_shader);
        }
    }
}
void Board::Play()
{
    // Czêstotliwoœæ odœwie¿ania ekanu
    glfwSwapInterval(4);
    // Tutaj dzieje siê renderowanie co klatkê -> g³ówna pêtla programu
    // Wykorzystanie ukrytej zmiennej, która informuje nas czy okno kontekstowe powinno siê zamkn¹æ
    while (!glfwWindowShouldClose(_window))
    {
        // Warunek sprawdzaj¹cy czy gra siê skoñczy³a
        if (!IsMovesLeft() || Player::_EvaluateBoard(_board) != 0)
        {
            // Wypisz zwyciêzce
            CongratsWinner();
            // Zresetuj planszê
            Reset();
        }
        // SprawdŸ czy dany gracz powinien wykonaæ ruch oraz czy mo¿na wykonaæ ruch po czym "oddaj mu kontrolê nad plansz¹"
        else if (IsMovesLeft() && _player_one->GetPriority())
        {
            if (_player_one->MakeMove(_window, _board)) NextTurn();
        }
        else if (IsMovesLeft() && _player_two->GetPriority())
        {
            if (_player_two->MakeMove(_window, _board)) NextTurn();
        }
        // Rysuj stan planszy
        PrintBoard();
        // Jeœli wciœniêty zostanie klawisz esc wy³¹cz okno
        _ProcessInput();
        // Zmieñ buffery do renderu oraz pilnuj eventów glfw
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
void Board::CongratsWinner()
{
    // SprawdŸ wartoœæ planszy i pogratuluj zwyciêzcy
    int score = Player::_EvaluateBoard(_board);
    std::cout << std::string(20, '+') << '\n'
              << (score == 10 ? "Player one has won!\n" : score == -10 ? "Player two has one!\n" : "Tie\n")
              << std::string(20, '+') << '\n';
}

void Board::_ProcessInput()
{
    // Co siê stanie po wœciœniêciu esc
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Ustaw ukryt¹ zmienn¹ tak aby okno kontekstowe mia³o siê zamkn¹æ
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }
}
void Board::_CreatePlayers()
{
    // Tworzenie gracza
    // Konsolowe zapytanie i zapisanie odpowiedzi
    int choice = 0;
    std::cout << "1: Gracz1 - Czlowiek, Gracz2 - Komputer\n"
        << "2: Gracz1 - Komputer, Gracz2 - Czlowiek\n"
        << "3: Gracz1 - Czlowiek, Gracz2 - Czlowiek\n"
        << "4. Gracz1 - Komputer, Gracz2 - Komputer\n"
        << "5. Wyjdz z gry\n"
        << "Twoj wybor: ";
    std::cin >> choice;

    // ¯eby nie by³o kolizji w destruktorze klasy Board
    if (choice != 5)
    {
        if (_player_one) delete _player_one;
        if (_player_two) delete _player_two;
    }

    // W zale¿noœci od opcji stwórz gracza
    switch (choice)
    {
        case 2:
        {
            _player_one = new Computer('X');
            _player_two = new Human('O');
        }
        break;
        case 3:
        {
            _player_one = new Human('X');
            _player_two = new Human('O');
        }
        break;
        case 4:
        {
            _player_one = new Computer('X');
            _player_two = new Computer('O');
        }
        break;
        // Wy³¹cz okno kontekstowe (ustaw ukryt¹ zmienn¹ operuj¹c¹ stanem okna)
        case 5: glfwSetWindowShouldClose(_window, GLFW_TRUE);
        break;
        case 1:
        default:
        {
            _player_one = new Human('X');
            _player_two = new Computer('O');
        }
    }
}