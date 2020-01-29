#include "pch.h"
#include "Algorithms.h"
#include <map>
#include <vector>

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
std::map<_Ty, std::vector<Cell<_Ty, _N>>> mapCellsToMarks(Unit<_Ty, _N> & unit) {
	auto output = std::map<_Ty, std::vector<Cell<_Ty, _N>>>();
	for (_Ty m = 1; m <= _N; m++) {
		output[m] = std::vector<Cell<_Ty, _N>>();
		for (_Ty i = 0; i < _N; ++i) {
			if (unit.cell(i).containsMark(m))
				output[m].push_back(unit.cell(i));
		}
	}
	return output;
};

ALGORITHM_TEMPLATE
std::vector<_Ty> markingIntersection(std::vector<Cell<_Ty, _N>> & arg0) {
	auto output = std::vector<_Ty>();
	auto start = std::find_if(arg0.begin(), arg0.end(), [](Cell<_Ty, _N> & cell) {
		return cell.isEmpty();
	});
	if (start == arg0.end())
		return output;

	for (_Ty i = 0, mark = (*start)[0]; mark != EMPTY_VALUE; ++i, mark = (*start)[i]) {
		for (auto it = start; it != arg0.end(); ++it) {
			if (!it->isEmpty())
				continue;
			if (it->containsMark(mark))
				output.push_back(mark);
		}
	}
	return output;
}

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::hiddenSequence(Unit<_Ty, _N> & unit) {
	/* https://www.sudokuwiki.org/Hidden_Candidates
	Conditions to satisfy:
	1. The hidden sequence's size must be > 0 and < the size of the unit.
	2. All cells in the hidden sequence must either be in the same row, column, or box.
	3. All cells in the hidden sequence must share n number of unique markings, if n is the size of the hidden sequence.

	If all the conditions are satisfied:
	- Remove all other markings from the cells in the hidden sequence.

	This algorithm finds the first hidden sequence and then stops (including hidden singles).
	*/
	bool mutated = false;
	auto markingMap = mapCellsToMarks(unit);
	for (auto it = markingMap.begin(); it != markingMap.end(); ++it) {
		if (it->second.size() == 1) {
			// We've found a hidden single! (we take those wins)
			if (it->second[0].setValue(it->first))
				return true;
		}
		if (it->second.size() < 2)
			continue;

		auto sharedMarkings = markingIntersection(it->second);
		if (sharedMarkings.size() <= 0)
			throw "Something when wrong with markingIntersection()";
		else if (sharedMarkings.size() != markingMap[it->first].size())
			continue;

		for (Cell<_Ty, _N> & cell : it->second) {
			mutated |= cell.resetMarks();
			for (int marking : sharedMarkings) {
				mutated |= cell.mark(marking);
			}
		}
		return mutated;
	}
	return mutated;
};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::hiddenSequence(Board<_Ty, _N> & board) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::intersectionRemoval(Board<_Ty, _N> & board) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::xwing(Board<_Ty, _N> & board) {};

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::ywing(Board<_Ty, _N> & board) {};




