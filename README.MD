Projekt stworzony w programie Visual Studio 2019

---Użyte zewnętrzne biblioteki---
1. GLFW (3.3.2) - https://www.glfw.org/download.html
2. GLAD (Version 4.6 / Core) - https://glad.dav1d.de
3. GLM (0.9.9.8) - https://github.com/g-truc/glm/releases/tag/0.9.9.8
4. SOIL2 (release-1.20) - https://github.com/SpartanJ/SOIL2/releases/tag/release-1.20

---O projeckie---
Projekt nazywa się GraphicsAPIs dlatego, że domyślnie miałem zamiar uczyć się jak działają różne biblioteki graficzne. Z czasem rozpisywania funkcjonalności na zajęciach pojawiło się zadanie końcowe -> projekt na zaliczenie. Struktura choć prosta została rozbudowana na tyle, że można mogłem ją wykorzystać w wykonaniu zadania - wystarczyło dodać tylko algorytm minimax oraz to kiedy ospowiednia grafika powinna się renderować. W taki sposób powstała klasa planszy i klasa gracza (z której dziedziczą człowiek i komputer). Plansza jest odpowiedzialna za rysowanie, resetowanie, monitorowanie stanu wewnętrznej tablicy, gratulowanie graczom czy zmianę tur. Gracze natomiast mogą wykonać odpowiedni ruch w natualny dla nich sposób (człowiek ma autonomiczne ruchy natomiast komputer dąży do uzyskania jak najlepszego rezultatu). W pętli rysującej upominają co chwilę planszę czy to jest ich kolej na ruch i bardzo mocno to egzekwują. Gracze wraz ze stworzeniem mają przypisany znak którym się posługują - jednocześnie dostają odpowiednią teksturę do użytku. Plansza przy rysowaniu tablicy zmienia co chwilę pozycję jednego elementu tak by wpasowywał się w dziewięć pól i sprawdza jaki jest przypisany znak do danego pola - prosi wtedy danego gracza o udzielenie dostępu do jego tekstury. Każda nowa gra rozpoczyna się przez interakcję z konsolą.

Po kompilacji powstaje folder z odpowiednią platformą docelową (będzie to x64). Zobaczymy tam folder DEBUG a w nim .exe. Jeśli chcemy uruchomić program z poziomu .exe to należy wcześniej skopiować folder res (ten sam katalog co .sln) i wkleić w ten sam katalog co .exe. Tyczy się to każego trybu kompilacji - jeśli chcemy odpalić bezpośrednio plik .exe to należy przekopiować folder res.

---Możliwe błędy---
Błędy z bibliotekami najlepiej rozwiązać poprzez ich ponowną instalację

--GLFW:
1.  https://www.glfw.org/download.html -> Source package
2.  Rozpakowjemy .zip
3.  Kopiujemy folder include, wracamy do foledru projektu, wklejamy wewnątrz Dependencies\GLFW-3.3.2 (zasepujemu include tym includem który przed chwilą skopiowaliśmy)
4.  Ściągamy i odpalamy narzędzie CMake - https://cmake.org/download/
5.  Jako folder źródłowy wybieramy rozpkowany wcześniej .zip, jako folder docelowy jakiś folder do którego będziemy się odwoływać - będę go nazywać "build"
6.  Klikamy Configure, wybieramy wersję VS (wersja 16 jest uznawana jako 2019)
7.  Wybieramy Generate
8.  W folderze build znajdziemy plik GLFW.sln, odpalamy go
9.  Kompilujemy rozwiązanie (skrót -> Ctrl+Shift+B)
10.  Wewnątrz build\src\Debug znajdziemy glfw3.lib, kopiujemy i wklejamy wewnątrz projektu Dependencies\GLFW-3.3.2\lib (zamieniamy tamtejszy glfw3.lib na ten skopiowany)
11.  Otwieramy projekt GraphicsAPIs, kompilujemy, jeśli się nie powiodło spójrz na linkowanie

--GLAD:
1.  https://glad.dav1d.de
2.  Wybierz odpowiednie opcje
      *) Language -> C/C++
      *) API -> gl -> Version 4.6
      *) Specification -> OpenGL
      *) Profile -> Core
      *) Options -> Generate a loader
3. Ściągnij wygenerowany plik .zip i go wypakuj -> powinniśmy otrzymać folder include i src
4. Wewnątrz VS (GraphicsAPIs) znajdź plik glad.c (Dependencies\GLAD\src) i wyklucz go z projektu (kliknij prawym przyciskiem myszy -> Wyklucz z projektu)
5. Wklejamy wcześniej wypakowane foldery (include i src z pkt 3) wewnątrz Dependencies\GLAD (zamieniamy tamtejsze foldery include i src na te skopiowane)
6. Wewnątrz VS (GraphicsAPIs) znajdujemy plik glad.c (Dependencies\GLAD\src) i dołącz go do projektu (kliknij prawym przyciskiem myszy -> Dołącz do projektu)
7. Kompilujemy, jeśli się nie powiodło spójrz na linkowanie

--GLM:
1. https://github.com/g-truc/glm/releases/tag/0.9.9.8
2. Rozpakowujemy .zip
3. Wewnątrz glm\glm skopiuj wszystko poza CMakeLists.txt i udaj się do projektu (GraphicsAPIs)
4. Wejdź w src\vendor\GLM\glm i wklej wcześniej skopiowane pliki (zamień obecne pliki na skopiowane)
5. Skompiluj projekt, jeśli się nie powiodło to spójrz na linkowanie

--SOIL2:
1. https://github.com/SpartanJ/SOIL2/releases/tag/release-1.20
2. Rozpakowujemy .zip
3. Ściągamy Premake - https://premake.github.io -> pre build binaries
4. Przenosimy premake.exe do rozpakowanego zipa (umieszczamy w tym samym folderze co plik o nazwie premake5.lua)
5. Otwieramy cmd wewnątrz tego folderu (z plikiem premake.exe) -> upewnij się, że jesteś w SOIL2-release-1.20
6. W konsoli wpisz:
premake5.exe vs2019
7. Wejdź w SOIL2-release-1.20\make\windows (platforma na której użyto premake5) i włącz SOIL2.sln
8. Wybierz projekt soil2-static-lib (eksplorator rozwiązań)
9. Ustaw konfigurację na debug x64, a następnie kompiluj
10. Powtórz krok 9 dla konfiguracji release x64 
11. Skopiuj z SOIL2-release-1.20\lib\windows pliki soil2.lib oraz soil2-debug.lib
12. Udaj się do projektu -> src\vendor\SOIL2\lib i wklej tam pliki z pkt 11 (podmnień obecne pliki na skopiowane)
13. Skompiluj projekt, jeśli się nie powiodło to spójrz na linkowanie

--Linkowanie:
Upewnij się, że program posiada odpowiednie linijki wewnątrz:

1. Właściwości -> C/C++ -> Ogólne -> Dodatkowe katalogi plików nagłówkowych
$(SolutionDir)Dependencies\GLFW-3.3.2\include
$(SolutionDir)Dependencies\GLAD\include
$(SolutionDir)Dependencies\GLAD\src
$(SolutionDir)src\vendor\SOIL2\include
$(SolutionDir)src\vendor\GLM

2. Właściwości -> Konsolidator -> Ogólne -> Dodatkowe katalogi biblioteki
$(SolutionDir)src\vendor\SOIL2\lib
$(SolutionDir)Dependencies\GLFW-3.3.2\lib

3. Właściwości -> Konsolidator -> Dane wejściowe -> Dodatkowe zależności
opengl32.lib
glfw3.lib
Dla konfiguracji DEBUG -> soil2-debug.lib
Dla konfiguracji RELEASE -> soil2.lib

Przy edycji bez użycia <Edit...> pamiętać o umieszczeniu średników po każdej linijce. W przeciwnym wypadku zapisać w każdej osobnej linijce.
Skompilować GraphicsAPIs. Jeśli dalej nie działa, spójrz na ostatnią sekcję.

--Inne
1. Jeśli projekt wyświetla się jako "zwolniony" (nie można go odczytać) to usuń plik .suo wewnątrz .vs (oba foldery są ukryte więc trzeba włączyć odpowiednią opcję w ekploratorze plików), a następnie prawym przyciskiem kliknij na GraphicsAPIs i załaduj ponownie. Jeśli dale nie działa usuń plik .user (w tym samym katalogu co .sln)
