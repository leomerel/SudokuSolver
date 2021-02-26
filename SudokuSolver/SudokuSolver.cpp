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

