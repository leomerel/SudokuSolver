#include "Sudoku.h"
#include "CSP.h"


int main()
{
    std::cout << "Hello World!\n";

    Sudoku un = Sudoku();
    un.display();

    CSP cehesspet = CSP(un);
}

