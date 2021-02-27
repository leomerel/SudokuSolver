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
    std::cout << "\n" << std::get<0>(assignement[0]) << std::get<1>(assignement[0]) << assignement.size() << "\n";
    Sudoku result = csp.variables;
    result.complete(assignement);
    return result;
}

int main()
{
    std::cout << "Hello World!\n";

    Sudoku un = Sudoku();
    un.display();

    CSP cehesspet = CSP(un);

    Sudoku result = backtrackingSearch(cehesspet);
    result.display();

    std::cout << recursiveCount;
}

