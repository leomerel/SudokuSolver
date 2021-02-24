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

private:
	void createConstraints();


};

