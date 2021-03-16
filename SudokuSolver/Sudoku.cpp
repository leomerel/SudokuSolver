#include "Sudoku.h"

Sudoku::Sudoku()
{
    //Sudoku de test
    grid = { { 0, 7, 2, 0, 0, 4, 3, 0, 5 },{ 0, 9, 0, 8, 5, 0, 2, 0, 0 },{ 0, 5, 8, 7, 2, 3, 1, 0, 0 },{ 3, 4, 0, 0, 7, 0, 9, 2, 0 },{ 9, 1, 0, 0, 8, 2, 0, 0, 6 },{ 0, 2, 6, 0, 0, 0, 0, 1, 7 },{ 0, 0, 0, 0, 4, 0, 8, 0, 3 },{ 7, 0, 1, 0, 0, 9, 0, 4, 0 },{ 5, 3, 4, 0, 6, 0, 0, 0, 0 } };
}

Sudoku::Sudoku(std::string filename)
{
    //Création de l'objet Sudoku à l'aide du fichier .ss
    std::ifstream myFile;
    myFile.open(filename);
    std::string line;

    std::vector<std::vector<int>> newGrid;

    if (myFile.is_open())
    {
        while (std::getline(myFile, line))
        {
            std::vector<int> newLine;
            for (char c : line)
            {
                if (c == '!' || c == '-')
                {
                    continue;
                }
                else if (c == '.')
                {
                    newLine.emplace_back(0);
                }
                else
                {
                    newLine.emplace_back(c - '0');
                }
            }
            if (newLine.size() == 9)
            {
                newGrid.emplace_back(newLine);
            }
        }
        myFile.close();
    }
    else std::cout << "Unable to open file\n";

    grid = newGrid;
}

void Sudoku::display()
{
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
                grid[i][j] != 0 ? line.append(std::to_string(grid[i][j]) + " ") : line.append(". ");
            }
            else
            {
                grid[i][j] != 0 ? line.append(std::to_string(grid[i][j]) + " ") : line.append(". ");
            }
        }
        line.append("|\n");
        std::cout << line;
    }
    std::cout << "\n";
}

void Sudoku::complete(std::vector<std::tuple<int, int>> assignement)
{
    for (auto assigned : assignement)
    {
        int i = std::get<0>(assigned) / 9;
        int j = std::get<0>(assigned) % 9;

        grid[i][j] = std::get<1>(assigned);
    }
}
