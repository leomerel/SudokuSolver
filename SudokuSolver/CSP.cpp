#include "CSP.h"

CSP::CSP(Sudoku sudo):variables(sudo)
{
	createConstraints();
	setUnassignedValues();
	initDomains();
}

bool CSP::checkConstraints(int var, int value, std::vector<std::tuple<int, int>> assignement)
{
	for (auto concernedConstraints : constraints)
	{
		if (std::get<0>(concernedConstraints) == var) 
		{
			int i = std::get<1>(concernedConstraints) / 9;
			int j = std::get<1>(concernedConstraints) % 9;
			if (value == variables.grid[i][j])
			{
				return false;
			}
			else
			{
				for (auto assigned : assignement)
				{
					if (std::get<0>(assigned) == std::get<1>(concernedConstraints) && value == std::get<1>(assigned))
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

int CSP::selectUnassignedVariable(std::vector<std::tuple<int, int>> assignement)
{
	bool alreadyAssigned = false;
	int mrvVar;
	int constraintsMrvVar = 0;
	int mrvVarDomainSize = 10000;

	for (int i = 0; i < unassignedValues.size(); i++)
	{
		alreadyAssigned = false;
		int var = unassignedValues[i];
		for (std::tuple<int, int> assigned : assignement)
		{
			if (var == std::get<0>(assigned))
			{
				alreadyAssigned = true;
				break;
			}
		}
		if (!alreadyAssigned)
		{
			if (domains[var].size() < mrvVarDomainSize)
			{
				mrvVar = var;
				mrvVarDomainSize = domains[mrvVar].size();
			}
			else if (domains[var].size() == domains[mrvVar].size())
			{
				int constraintsVar = 0;
				for (auto concernedConstraints : constraints)
				{
					if (std::get<0>(concernedConstraints) == var)
					{
						bool alreadyAssignedDHvar = false;
						for (std::tuple<int, int> assigned : assignement)
						{
							if (std::get<1>(concernedConstraints) == std::get<0>(assigned))
							{
								alreadyAssignedDHvar = true;
								break;
							}
						}
						if (variables.grid[std::get<1>(concernedConstraints) / 9][std::get<1>(concernedConstraints) % 9] == 0 || !alreadyAssignedDHvar)
						{
							constraintsVar += 1;
						}
					}
				}
				if (constraintsVar > constraintsMrvVar)
				{
					mrvVar = var;
					constraintsMrvVar = constraintsVar;
					mrvVarDomainSize = domains[mrvVar].size();
				}
			}
		}
	}
	return mrvVar;
}

void CSP::setUnassignedValues()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++) {
			if (variables.grid[i][j] == 0)
			{
				unassignedValues.emplace_back(i * 9 + j);
			}
		}
	}

}

void CSP::createConstraints()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++) 
		{			
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

void CSP::initDomains()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (variables.grid[i][j] == 0)
			{
				domains.emplace_back(std::vector<int>({ 1,2,3,4,5,6,7,8,9 }));
			}
			else
			{
				domains.emplace_back(std::vector<int>({}));
			}
		}
	}
}


