// SudokuSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Cell.h"
#include "Unit.h"
#include "Board.h"

int main() {
	using _Ty = unsigned char;
	const _Ty _N = 9;

	std::array<_Ty, _N * _N> raw {
		0, 6, 0, 0, 4, 0, 0, 5, 0,
		0, 5, 0, 0, 0, 0, 0, 8, 9,
		0, 0, 3, 5, 0, 0, 4, 0, 0,

		0, 0, 0, 0, 0, 1, 3, 0, 0,
		9, 0, 0, 8, 0, 7, 0, 0, 1,
		0, 0, 6, 2, 0, 0, 0, 0, 0,

		0, 0, 9, 0, 0, 2, 7, 0, 0,
		8, 4, 0, 0, 0, 0, 0, 6, 0,
		0, 7, 0, 0, 1, 0, 0, 3, 0
	};

	Board<_Ty, _N> board { raw };

	std::cout << "Hello World!" << std::endl;
}