#include "Sudoku.h"
#include "CSP.h"

int recursiveCount = 0;

bool removeInconsistentValues(std::tuple<int, int> constraint, CSP* csp)
{
    int xi = std::get<0>(constraint);
    std::vector<int> xiDomain = csp->domains[xi];

    for (int value : xiDomain)
    {
        int xj = std::get<1>(constraint);

        if (csp->variables.grid[xj / 9][xj % 9] == value)
        {
            csp->domains[xi].erase(std::find(csp->domains[xi].begin(), csp->domains[xi].end(), value));
            return true;
        }
        if (csp->domains[xj].size() == 1 && csp->domains[xj][0] == value)
        {
            csp->domains[xi].erase(std::find(csp->domains[xi].begin(), csp->domains[xi].end(), value));
            return true;
        }
    }

    return false;
}

CSP AC3(CSP initCsp)
{
    std::vector<std::tuple<int, int>> queue = initCsp.constraints;
    while (queue.size() > 0)
    {
        std::tuple<int, int> firstContraint = queue[0];
        queue.erase(queue.begin());
        if (removeInconsistentValues(firstContraint, &initCsp))
        {
            for (std::tuple<int,int> neighbors : initCsp.constraints)
            {
                if (std::get<1>(neighbors) == std::get<0>(firstContraint))
                {
                    queue.emplace_back(neighbors);
                }
            }
        }
    }
    return initCsp;
}

std::vector<std::tuple<int,int>> recursiveBacktracking(std::vector<std::tuple<int,int>> assignement, CSP csp)
{
    recursiveCount++;
    if (assignement.size() == csp.unassignedValues.size())
    {
        std::cout << "Completed sudoku\n";
        return assignement; //assignement sous la forme d'un sudoku complété
    }

    int var = csp.selectUnassignedVariable(assignement);
    for (int value : csp.domains[var])
    {
        //std::cout << "\nSelected case : " << var << " with value " << value << " on iteration n" << recursiveCount;

        if (csp.checkConstraints(var, value, assignement))
        {
            assignement.emplace_back(std::make_tuple(var, value));
            CSP modifiedCSP = AC3(csp);
            std::vector<std::tuple<int, int>> result = recursiveBacktracking(assignement, modifiedCSP);
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

