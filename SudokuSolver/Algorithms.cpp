#include "pch.h"
#include "Algorithms.h"

#define EMPTY_VALUE Cell<_Ty, _N>::EMPTY_VALUE

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::removeMarkings(Unit<_Ty, _N> & unit) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		Cell<_Ty, _N> cell = unit.cell(i);
		if (cell.isEmpty())
			continue;

		for (_Ty k = 0; k < _N; ++k) {
			Cell<_Ty, _N> cell2 = unit.cell(i);
			if (!cell2.isEmpty() || cell == cell2)
				continue;

			mutated |= cell2.unmark(cell.getValue());
		}
	}
	return mutated;
}

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::removeMarkings(Board<_Ty, _N> & board) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		mutated |= removeMarkings(board.row(i));
		mutated |= removeMarkings(board.col(i));
		mutated |= removeMarkings(board.box(i));
	}
	return mutated;
};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::nakedSingle(Unit<_Ty, _N> & unit) {
	/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
	If found, that cell's value becomes that mark.

	This algorithm is just a slightly worse version of the hiddenSingle algorithm. While the hidden single algorithm does do the same thing as this (and more),
	this algorithm does it's job slightly faster than the hiddenSingle algorithm, so there is a trade-off there.
	*/
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		Cell<_Ty, _N> cell = unit.cell(i);
		if (!cell.isEmpty())
			continue;

		if (cell[1] == EMPTY_VALUE)
			mutated |= cell.setValue(cell[0]);
	}
	return mutated;
};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::nakedSingle(Board<_Ty, _N> & board) {
	/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
	If found, that cell's value becomes that mark.

	This algorithm is just a slightly worse version of the hiddenSingle algorithm. While the hidden single algorithm does do the same thing as this (and more),
	this algorithm does it's job slightly faster than the hiddenSingle algorithm, so there is a trade-off there.
	*/
	bool mutated = false;
	for (_Ty i = 0; i < _N * _N; ++i) {
		Cell<_Ty, _N> cell = board.cell(i);
		if (!cell.isEmpty())
			continue;

		if (cell[1] == EMPTY_VALUE)
			mutated |= cell.setValue(cell[0]);
	}
	return mutated;
}

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::hiddenSingle(Unit<_Ty, _N> & unit) {
	/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
	If found, that cell's value becomes that mark.
	*/
	bool mutated = false;
	for (_Ty m = 1; m <= _N; ++m) {
		auto func = [&m](Cell<_Ty, _N> & cell) {
			return cell.containsMark(m);
		};

		auto first = std::find_if(unit.cells.begin(), unit.cells.end(), func);
		auto last = std::find_if(unit.cells.rbegin(), unit.cells.rend(), func);
		if (first == last) {
			mutated |= first->setValue(m);
		}
	}
	return mutated;
};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::hiddenSingle(Board<_Ty, _N> & board) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		mutated |= board.row(i);
		mutated |= board.col(i);
		mutated |= board.box(i);
	}
	return mutated;
};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::hiddenSequence(Unit<_Ty, _N> & unit) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::hiddenSequence(Board<_Ty, _N> & board) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::intersectionRemoval(Board<_Ty, _N> & board) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::xwing(Board<_Ty, _N> & board) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::ywing(Board<_Ty, _N> & board) {};




