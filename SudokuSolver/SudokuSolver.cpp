#include "Sudoku.h"
#include "CSP.h"
#include <deque>
#include <algorithm>

int recursiveCount = 0;
bool ac3 = true;
bool leastContrainingValue = true;
bool mrv = true;

std::vector<int> orderDomainValue(int var, std::vector<std::tuple<int, int>> assignement, CSP csp)
{
    std::vector<int> orderDomainValue;
    std::vector<std::tuple<int, int>> orderDomainTuple;

    for (int value : csp.domains[var])
    {
        std::vector<std::tuple<int, int>> localAssignement = assignement;
        localAssignement.emplace_back(std::make_tuple(var, value));
        std::vector <int> unassignedValues;
        int remainDomainSize = 0;

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++) {
                if (csp.variables.grid[i][j] == 0)
                {
                    bool alreadyassigned = false;
                    for (auto assigned : localAssignement)
                    {
                        if (std::get<0>(assigned) == i * 9 + j)
                        {
                            alreadyassigned = true;
                        }
                    }

                    if (!alreadyassigned)
                    {
                        remainDomainSize += csp.domains[i * 9 + j].size();
                    }
                }
            }
        }

        orderDomainTuple.emplace_back(std::make_tuple(value, remainDomainSize)); 
    }

    std::sort(orderDomainTuple.begin(), orderDomainTuple.end(),
        [](const std::tuple<int, int>& a,
            const std::tuple<int, int>& b) { return (std::get<1>(a) < std::get<1>(b)); });

    for (auto tuple : orderDomainTuple)
    {
        orderDomainValue.emplace_back(std::get<0>(tuple));
    }


    return orderDomainValue;
}

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
    //std::vector<std::tuple<int, int>> queue = initCsp.constraints;
    std::deque<std::tuple<int, int>> queue;
    std::copy(initCsp.constraints.begin(), initCsp.constraints.end(), std::inserter(queue, queue.end()));
    while (queue.size() > 0)
    {
        std::tuple<int, int> firstContraint = queue[0];
        //queue.erase(queue.begin());
        queue.pop_front();
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

    int var = csp.selectUnassignedVariable(assignement,mrv);
    std::vector<int> orderListDomain = orderDomainValue(var, assignement, csp);
    for (int value : (leastContrainingValue? orderListDomain : csp.domains[var]))
    {
        //std::cout << "\nSelected case : " << var << " with value " << value << " on iteration n" << recursiveCount;

        if (csp.checkConstraints(var, value, assignement))
        {
            assignement.emplace_back(std::make_tuple(var, value));
            std::vector<std::tuple<int, int>> result;
            if (ac3)
            {
                CSP modifiedCSP = AC3(csp);
                result = recursiveBacktracking(assignement, modifiedCSP);
            }
            else
            {
                result = recursiveBacktracking(assignement, csp);
            }
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
    std::ifstream recordFile;
    bool solve = true;
    bool ending = true;

    while (solve)
    {
        do
        {
            if (ending)
            {
                std::cout << "Parametres du backtracking : \nAC-3 : " << std::to_string(ac3) << "\nMinimum Remaining Value et Degree Heuristic : " <<
                    std::to_string(mrv) << "\nLeast Constraining Value : " << std::to_string(leastContrainingValue);

                std::cout << "\nVoulez vous changer les parametres du backtracking ? (y/n)";
                std::string change;
                std::cin >> change;
                if (change == "y" || change == "Y" || change == "yes")
                {
                    std::cout << "\nIndiquer 'n' pour desactiver (actif par defaut)";
                    std::string strAc3, strMRV, strLCV;
                    std::cout << "\nAC-3 : ";
                    std::cin >> strAc3;
                    std::cout << "\nMinimum Remaining Value et Degree Heuristic : ";
                    std::cin >> strMRV;
                    std::cout << "\nLeast Constraining Value : ";
                    std::cin >> strLCV;

                    strAc3 == "n" ? ac3 = false : ac3 = true;
                    strMRV == "n" ? mrv = false : mrv = true;
                    strLCV == "n" ? leastContrainingValue = false : leastContrainingValue = true;

                    //nettoyage de la console
                    #if defined _WIN32
                                        system("cls");
                    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
                                        system("clear");
                    #elif defined (__APPLE__)
                                        system("clear");
                    #endif

                    std::cout << "Parametres du backtracking : \nAC-3 : " << std::to_string(ac3) << "\nMinimum Remaining Value et Degree Heuristic : " <<
                        std::to_string(mrv) << "\nLeast Constraining Value : " << std::to_string(leastContrainingValue);
                }

                std::cout << "\n\nEntrez le nom du fichier .ss a utiliser : ";
                std::cin >> filename;
            }
            else
            {
                std::cout << "Le nom du sudoku n'existe pas veuillez reessayer : ";
                std::cin >> filename;
            }

            ending = false;

            while (filename.size() <= 3)
            {
                std::cout << "\nLe nom du fichier est trop court (minimum 3 caracteres)";
                std::cout << "\nEntrez le nom du fichier .ss a utiliser : ";
                std::cin >> filename;
            }

            std::string end = ".ss";

            if (filename.compare(filename.size() - 3, 3, end) != 0)
            {
                filename.append(end);
            }
            recordFile.open(filename);

            if (recordFile.good())
            {
                ending = true;
                recordFile.close();
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
                    break;
                }
            }
        }         
        while (recordFile.fail());
        {
            if (!ending)
            {
                std::cout << "Le nom du sudoku n'existe pas veuillez reessayer : ";
                std::cin >> filename;
                recordFile.open(filename);
            }
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

 