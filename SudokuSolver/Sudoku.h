#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <iostream>

class Sudoku
{
public:
	Sudoku();

	void display();

	std::vector<std::vector<int>> grid;

	void complete(std::vector<std::tuple<int, int>> assignement);

};

