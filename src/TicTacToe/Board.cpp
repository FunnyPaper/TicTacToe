#include "Board.h"
#include <iostream>

Board::Board(GLFWwindow* window) : _window(window), _player_one(nullptr), _player_two(nullptr)
{
    // Stwórz shader w oparciu o lokazlizację dwóch plików -> vertex i fragment shader
    _shader = new Shader("res/shaders/vs.shader", "res/shaders/fs.shader");
    // Przypisujemy nazwy slotów tekstur do numerycznych wartości (tworzenie łączeń)
    _shader->SetUniform1i("diffuse_texture0", 0);
    _shader->SetUniform1i("specular_texture1", 1);
    // Stwórz tymczasową kamerę i prześlij ustawienia do shadera
    // Nie potrzebujemy kontroli nad kamerą -> wystarczy nam jej bazowe ustawienie po czym możemy o niej zapomnieć
    Camera(glm::vec3(0.0f, 0.0f, 0.0f)).SendToShader(_shader);
    // Stwórz pojedyncze pole
    // Zmienne do manipulacji skalą pola oraz odstępami
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
    // Ustaw stan początkowy planszy (wypełnianie tablicy pustymi wartościami)
    Reset();
}
Board::~Board()
{
    // Destruktor
    // Usuń dynamiczne obiekty
    if (_player_one) delete _player_one;
    if (_player_two) delete _player_two;
    if (_tile) delete _tile;
    if (_shader) delete _shader;
}
void Board::Reset()
{
    // Stwórz nowych graczy
    _CreatePlayers();
    // Czyścimy ekran co klatkę
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Przestaw ustawienia shadera
    _shader->SetUniform1i("is_texture_attached", 0);
    // Dezaktywuj ostatnią teksturę
    Texture::Unbind();
    // W pętli ustaw stan początkowy planszy (pusta) oraz narysuj pustą planszę w oknie kontekstowym
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
    // Wywołaj następną turę
    // Zmień to kto powinien wykonać ruch
    _player_one->SwapPriority(); _player_two->SwapPriority();
}
bool Board::IsMovesLeft()
{
    // Sprawdź czy plansza jest pusta
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
            // W zależności od zawartości komórki zmień ustawienia shadera oraz aktywuj odpowiednią teksturę
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
            // Zmień pozycję obiektu oraz go narysuj
            _tile->SetPosition(_tile_position[i][j]);
            _tile->Render(_shader);
        }
    }
}
void Board::Play()
{
    // Częstotliwość odświeżania ekanu
    glfwSwapInterval(4);
    // Tutaj dzieje się renderowanie co klatkę -> główna pętla programu
    // Wykorzystanie ukrytej zmiennej, która informuje nas czy okno kontekstowe powinno się zamknąć
    while (!glfwWindowShouldClose(_window))
    {
        // Warunek sprawdzający czy gra się skończyła
        if (!IsMovesLeft() || Player::_EvaluateBoard(_board) != 0)
        {
            // Wypisz zwycięzce
            CongratsWinner();
            // Zresetuj planszę
            Reset();
        }
        // Sprawdź czy dany gracz powinien wykonać ruch oraz czy można wykonać ruch po czym "oddaj mu kontrolę nad planszą"
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
        // Jeśli wciśnięty zostanie klawisz esc wyłącz okno
        _ProcessInput();
        // Zmień buffery do renderu oraz pilnuj eventów glfw
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
void Board::CongratsWinner()
{
    // Sprawdź wartość planszy i pogratuluj zwycięzcy
    int score = Player::_EvaluateBoard(_board);
    std::cout << std::string(20, '+') << '\n'
              << (score == 10 ? "Player one has won!\n" : score == -10 ? "Player two has one!\n" : "Tie\n")
              << std::string(20, '+') << '\n';
}

void Board::_ProcessInput()
{
    // Co się stanie po wściśnięciu esc
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Ustaw ukrytą zmienną tak aby okno kontekstowe miało się zamknąć
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

    // Żeby nie było kolizji w destruktorze klasy Board
    if (choice != 5)
    {
        if (_player_one) delete _player_one;
        if (_player_two) delete _player_two;
    }

    // W zależności od opcji stwórz gracza
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
        // Wyłącz okno kontekstowe (ustaw ukrytą zmienną operującą stanem okna)
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
