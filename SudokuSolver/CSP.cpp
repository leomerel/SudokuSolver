#include "CSP.h"

CSP::CSP(Sudoku sudo):variables(sudo)
{
	createConstraints();
}

void CSP::createConstraints()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++) {
			
			if (variables.grid[i][j] == 0) 
			{
				int pos = i * 9 + j;

				for (int k = 0; k < 9; k++) {
					if (k != j) 
					{
						constraints.emplace_back(std::make_tuple(pos, i * 9 + k));
					}
					if (k != i)
					{
						constraints.emplace_back(std::make_tuple(pos, k * 9 + j));
					}
				}

				int resi = i / 3;
				int resj = j / 3;

				int posTuple = resi * 3 * 9 + resj * 3;

				std::vector<int> offsetList{ 0,1,2,9,10,11,18,19,20 };
				for (int offset : offsetList)
				{
					int posTupleOffset = posTuple + offset;

					if (pos != posTupleOffset && i != posTupleOffset / 9 && j != posTupleOffset % 9)
					{
						constraints.emplace_back(std::make_tuple(pos, posTupleOffset));
					}
				}				
			}			
		}
	}
}


