#include "Board.h"
#include <iostream>

Board::Board(GLFWwindow* window) : _window(window)
{
    // Stw�rz shader w oparciu o lokazlizacj� dw�ch plik�w -> vertex i fragment shader
    _shader = new Shader("res/shaders/vs.shader", "res/shaders/fs.shader");
    // Przypisujemy nazwy slot�w tekstur do numerycznych warto�ci (tworzenie ��cze�)
    _shader->SetUniform1i("diffuse_texture0", 0);
    _shader->SetUniform1i("specular_texture1", 1);
    // Stw�rz tymczasow� kamer� i prze�lij ustawienia do shadera
    // Nie potrzebujemy kontroli nad kamer� -> wystarczy nam jej bazowe ustawienie po czym mo�emy o niej zapomnie�
    Camera(glm::vec3(0.0f, 0.0f, 0.0f)).SendToShader(_shader);
    // Stw�rz pojedyncze pole
    // Zmienne do manipulacji skal� pola oraz odst�pami
    float _scaling_factor = 1.65f, _spaces = 0.06f;
    _tile = new Mesh(new Quad(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(_scaling_factor));
    // Przypisz rozmieszczenie p�l planszy
    _tile_position[0][0] = glm::vec3(-_scaling_factor - _spaces, _scaling_factor + _spaces, 0.0f);
    _tile_position[0][1] = glm::vec3(0.0f, _scaling_factor + _spaces, 0.0f);
    _tile_position[0][2] = glm::vec3(_scaling_factor + _spaces, _scaling_factor + _spaces, 0.0f);
    _tile_position[1][0] = glm::vec3(-_scaling_factor - _spaces, 0.0f, 0.0f);
    _tile_position[1][1] = glm::vec3(0.0f, 0.0f, 0.0f);
    _tile_position[1][2] = glm::vec3(_scaling_factor + _spaces, 0.0f, 0.0f);
    _tile_position[2][0] = glm::vec3(-_scaling_factor - _spaces, -_scaling_factor - _spaces, 0.0f);
    _tile_position[2][1] = glm::vec3(0.0f, -_scaling_factor - _spaces, 0.0f);
    _tile_position[2][2] = glm::vec3(_scaling_factor + _spaces, -_scaling_factor - _spaces, 0.0f);
    // Ustaw stan pocz�tkowy planszy (wype�nianie tablicy pustymi warto�ciami)
    Reset();
}
Board::~Board()
{
    // Destruktor
    // Usu� dynamiczne obiekty
    if (_player_one) delete _player_one;
    if (_player_two) delete _player_two;
    if (_tile) delete _tile;
    if (_shader) delete _shader;
}
void Board::Reset()
{
    // Stw�rz nowych graczy
    _CreatePlayers();
    // Czy�cimy ekran co klatk�
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Przestaw ustawienia shadera
    _shader->SetUniform1i("is_texture_attached", 0);
    // Dezaktywuj ostatni� tekstur�
    Texture::Unbind();
    // W p�tli ustaw stan pocz�tkowy planszy (pusta) oraz narysuj pust� plansz� w oknie kontekstowym
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
    // Wywo�aj nast�pn� tur�
    // Zmie� to kto powinien wykona� ruch
    _player_one->SwapPriority(); _player_two->SwapPriority();
}
bool Board::IsMovesLeft()
{
    // Sprawd� czy plansza jest pusta
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
            // W zale�no�ci od zawarto�ci kom�rki zmie� ustawienia shadera oraz aktywuj odpowiedni� tekstur�
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
            // Zmie� pozycj� obiektu oraz go narysuj
            _tile->SetPosition(_tile_position[i][j]);
            _tile->Render(_shader);
        }
    }
}
void Board::Play()
{
    // Cz�stotliwo�� od�wie�ania ekanu
    glfwSwapInterval(4);
    // Tutaj dzieje si� renderowanie co klatk� -> g��wna p�tla programu
    // Wykorzystanie ukrytej zmiennej, kt�ra informuje nas czy okno kontekstowe powinno si� zamkn��
    while (!glfwWindowShouldClose(_window))
    {
        // Warunek sprawdzaj�cy czy gra si� sko�czy�a
        if (!IsMovesLeft() || Player::_EvaluateBoard(_board) != 0)
        {
            // Wypisz zwyci�zce
            CongratsWinner();
            // Zresetuj plansz�
            Reset();
        }
        // Sprawd� czy dany gracz powinien wykona� ruch oraz czy mo�na wykona� ruch po czym "oddaj mu kontrol� nad plansz�"
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
        // Je�li wci�ni�ty zostanie klawisz esc wy��cz okno
        _ProcessInput();
        // Zmie� buffery do renderu oraz pilnuj event�w glfw
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
void Board::CongratsWinner()
{
    // Sprawd� warto�� planszy i pogratuluj zwyci�zcy
    int score = Player::_EvaluateBoard(_board);
    std::cout << std::string(20, '+') << '\n'
              << (score == 10 ? "Player one has won!\n" : score == -10 ? "Player two has one!\n" : "Tie\n")
              << std::string(20, '+') << '\n';
}

void Board::_ProcessInput()
{
    // Co si� stanie po w�ci�ni�ciu esc
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Ustaw ukryt� zmienn� tak aby okno kontekstowe mia�o si� zamkn��
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

    // �eby nie by�o kolizji w destruktorze klasy Board
    if (choice != 5)
    {
        if (_player_one) delete _player_one;
        if (_player_two) delete _player_two;
    }

    // W zale�no�ci od opcji stw�rz gracza
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
        // Wy��cz okno kontekstowe (ustaw ukryt� zmienn� operuj�c� stanem okna)
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