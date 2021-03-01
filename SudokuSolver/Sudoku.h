#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <fstream>

class Sudoku
{
public:
	Sudoku();
	Sudoku(std::string filename);

	void display();

	std::vector<std::vector<int>> grid;

	void complete(std::vector<std::tuple<int, int>> assignement);

};

