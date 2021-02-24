#include "Sudoku.h"

Sudoku::Sudoku()
{
	/*grid.emplace_back(new std::vector<int>({ 0, 7, 2, 0, 0, 4, 3, 0, 5 }));
	grid.emplace_back(new std::vector<int>({ 0, 9, 0, 8, 5, 0, 2, 0, 0 }));
	grid.emplace_back(new std::vector<int>({ 0, 5, 8, 7, 2, 3, 1, 0, 0 }));
	grid.emplace_back(new std::vector<int>({ 3, 4, 0, 0, 7, 0, 9, 2, 0 }));
	grid.emplace_back(new std::vector<int>({ 9, 1, 0, 0, 8, 2, 0, 0, 6 }));
	grid.emplace_back(new std::vector<int>({ 0, 2, 6, 0, 0, 0, 0, 1, 7 }));
	grid.emplace_back(new std::vector<int>({ 0, 0, 0, 0, 4, 0, 8, 0, 3 }));
	grid.emplace_back(new std::vector<int>({ 7, 0, 1, 0, 0, 9, 0, 4, 0 }));
	grid.emplace_back(new std::vector<int>({ 5, 3, 4, 0, 6, 0, 0, 0, 0 }));*/

    grid = { { 0, 7, 2, 0, 0, 4, 3, 0, 5 },{ 0, 9, 0, 8, 5, 0, 2, 0, 0 },{ 0, 5, 8, 7, 2, 3, 1, 0, 0 },{ 3, 4, 0, 0, 7, 0, 9, 2, 0 },{ 9, 1, 0, 0, 8, 2, 0, 0, 6 },{ 0, 2, 6, 0, 0, 0, 0, 1, 7 },{ 0, 0, 0, 0, 4, 0, 8, 0, 3 },{ 7, 0, 1, 0, 0, 9, 0, 4, 0 },{ 5, 3, 4, 0, 6, 0, 0, 0, 0 } };
}

void Sudoku::display()
{
    // Nettoyage de la console
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif

    // Affichage de l'etat courant du sudoku
    for (int i = 0; i < 9; i++) {
        std::string line;
        if (i % 3 == 0 && i != 0)
        {
            line.append("-------------------------\n");
        }
        for (int j = 0; j < 9; j++) {
            if ((j % 3) == 0) 
            {
                line.append("| ");
                line.append(std::to_string(grid[i][j]) + " ");
            }
            else
            {
                line.append(std::to_string(grid[i][j]) + " ");
            }
        }
        line.append("|\n");
        std::cout << line;
    }
    std::cout << "\n";
}
