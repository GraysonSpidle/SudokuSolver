// SudokuSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <vector>
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <climits>

#include "Cell.h"
#include "Unit.h"
#include "Board.h"
#include "Algorithms.h"


SUDOKU_TEMPLATE
bool testBoardLayout(AbstractBoard<_Ty, _N> & board, std::array<_Ty, _N * _N> & raw);

SUDOKU_TEMPLATE
std::vector<_Ty> getMarkings(AbstractCell<_Ty, _N> & cell);

SUDOKU_TEMPLATE
void printMarkings(AbstractBoard<_Ty, _N> & board);

SUDOKU_TEMPLATE
void printBoard(AbstractBoard<_Ty, _N> & board);

int main() {
	using _Ty = unsigned char;
	const _Ty _N = 9;

	std::array<_Ty, _N * _N> raw {
		0, 6, 0,  0, 4, 0,  0, 5, 0,
		0, 5, 0,  0, 0, 0,  0, 8, 9,
		0, 0, 3,  5, 0, 0,  4, 0, 0,

		0, 0, 0,  0, 0, 1,  3, 0, 0,
		9, 0, 0,  8, 0, 7,  0, 0, 1,
		0, 0, 6,  2, 0, 0,  0, 0, 0,

		0, 0, 9,  0, 0, 2,  7, 0, 0,
		8, 4, 0,  0, 0, 0,  0, 6, 0,
		0, 7, 0,  0, 1, 0,  0, 3, 0
	};

	Board<_Ty, _N> board = { raw };
	
	std::cout << SudokuAlgorithms::removeMarkings(board) << std::endl;
	while (SudokuAlgorithms::intersectionRemoval(board)) {
		std::cout << "Hidden Sequence Algorithm used" << std::endl;
	}
	
	printMarkings(board);
	printBoard(board);
	


	


}


SUDOKU_TEMPLATE
bool testBoardLayout(AbstractBoard<_Ty, _N> & board, std::array<_Ty, _N * _N> & raw) {
	bool output = true;

	// Testing if the cells in the board's rows and cols correspond to the same cell in memory
	for (_Ty i = 0; i < _N * _N; ++i) {
		AbstractCell<_Ty, _N> & cell = board.cell(i);
		_Ty row = cell.getRowIndex();
		_Ty col = cell.getColIndex();
		_Ty box = cell.getBoxIndex();
		
		if (cell != board.row(row)[col]) {
			std::cerr << "Cell r: " << std::to_string(row) << " c: " << std::to_string(col) << " in board.row() doesn't reference the same portion of memory as the master list of cells." << std::endl;
			output = false;
		}

		if (cell != board.col(col)[row]) {
			std::cerr << "Cell r: " << std::to_string(row) << " c: " << std::to_string(col) << " in board.col() doesn't reference the same portion of memory as the master list of cells." << std::endl;
			output = false;
		}

		if (cell != board.box(box).cell(row, col)) {
			std::cerr << "Cell r: " << std::to_string(row) << " c: " << std::to_string(col) << " in board.box() doesn't reference the same portion of memory as the master list of cells." << std::endl;
			output = false;
		}

		
	}


	return output;
}

SUDOKU_TEMPLATE
std::vector<_Ty> getMarkings(AbstractCell<_Ty, _N> & cell) {
	std::vector<_Ty> output = {};
	for (_Ty i = 0; i < _N; ++i) {
		if (cell.containsMark(i + 1))
			output.push_back(i + 1);
	}
	return output;
}

SUDOKU_TEMPLATE
void printMarkings(AbstractBoard<_Ty, _N> & board) {
	for (_Ty r = 0; r < _N; ++r) {
		for (_Ty c = 0; c < _N; ++c) {
			std::vector<_Ty> markings = getMarkings(board.row(r)[c]);
			std::cout << "r: " << std::to_string(r) << " c: " << std::to_string(c) << "  ";
			for (_Ty m : markings) {
				std::cout << std::to_string(m) << " ";
			}
			std::cout << std::endl;
		}
	}

}

SUDOKU_TEMPLATE
void printBoard(AbstractBoard<_Ty, _N> & board) {
	for (_Ty r = 0; r < _N; r++) {
		for (_Ty c = 0, b2 = SudokuUtils::calculateBoxIndex(r, c, _N), previousB2 = b2; c < _N; c++, b2 = SudokuUtils::calculateBoxIndex(r,c,_N)) {
			if (b2 != previousB2) {
				std::cout << "  ";
				previousB2 = b2;
			}
			std::cout << std::to_string(board.cell(r,c).getValue()) << " ";
		}
		std::cout << std::endl << std::endl;
	}
}
