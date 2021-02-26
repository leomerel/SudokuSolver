#pragma once
#include <tuple>
#include "Sudoku.h"

class CSP
{
public:
	CSP(Sudoku sudo);

	Sudoku variables;
	std::vector<int> domain { 1,2,3,4,5,6,7,8,9 };

	std::vector<std::tuple<int,int>> constraints;
	bool checkConstraints(int var, int value, std::vector<std::tuple<int, int>> assignement);

	int selectUnassignedVariable(std::vector<std::tuple<int,int>> assignement);

	std::vector<int> unassignedValues;
	void setUnassignedValues();

private:
	void createConstraints();

};

