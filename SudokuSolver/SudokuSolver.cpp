#include "Sudoku.h"
#include "CSP.h"

int recursiveCount = 0;

std::vector<std::tuple<int,int>> recursiveBacktracking(std::vector<std::tuple<int,int>> assignement, CSP csp)
{
    recursiveCount++;
    if (assignement.size() == csp.unassignedValues.size())
    {
        std::cout << "Completed sudoku\n";
        return assignement; //assignement sous la forme d'un sudoku complété
    }

    int var = csp.selectUnassignedVariable(assignement);
    for (int value : csp.domain)
    {
        if (csp.checkConstraints(var, value, assignement))
        {
            assignement.emplace_back(std::make_tuple(var, value));
            std::vector<std::tuple<int, int>> result = recursiveBacktracking(assignement, csp);
            if (std::get<1>(result[0]) != 0)
            {
                return result;
            }
            assignement.pop_back();
        }
    }
    return { std::make_tuple(0,0) };
}

Sudoku backtrackingSearch(CSP csp)
{
    std::vector<std::tuple<int, int>> assignement = recursiveBacktracking({}, csp);
    Sudoku result = csp.variables;
    result.complete(assignement);
    return result;
}

int main()
{
    std::cout << "Bienvenue sur SudokuSolver !\n";
    std::string filename;
    bool solve = true;

    while (solve)
    {
        std::cout << "\nEntrez le nom du fichier .ss a utiliser : ";
        std::cin >> filename;

        std::string end = ".ss";

        if (filename.compare(filename.size() - 3, 3, end) != 0)
        {
            filename.append(end);
        }

        Sudoku un = Sudoku(filename);
        un.display();

        CSP csp = CSP(un);

        Sudoku result = backtrackingSearch(csp);
        result.display();

        std::cout << recursiveCount << " executions de la boucle recursive.\n";

        std::cout << "Voulez vous tester un autre sudoku ? (y/n)";
        std::string other;
        std::cin >> other;
        if (other == "n" || other == "N" || other == "no")
        {
            solve = false;
        }
   
        //nettoyage de la console
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif

    }
    
}

