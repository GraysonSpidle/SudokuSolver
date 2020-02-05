#pragma once
#include <map>
#include <vector>

#include "AbstractUnit.h"
#include "AbstractBoard.h"

#define EMPTY_VALUE 0

SUDOKU_TEMPLATE
std::map<_Ty, std::vector<AbstractCell<_Ty, _N>*>> mapCellsToMarks(AbstractUnit<_Ty, _N> & unit) {
	auto output = std::map<_Ty, std::vector<AbstractCell<_Ty, _N>*>>();
	for (_Ty m = 1; m <= _N; m++) {
		for (_Ty i = 0; i < _N; ++i) {
			if (unit.cell(i).containsMark(m))
				output[m].push_back(&unit.cell(i));
		}
	}
	return output;
};

SUDOKU_TEMPLATE
std::map<_Ty, std::vector<AbstractCell<_Ty, _N>*>> mapCellsToMarks(AbstractBoard<_Ty, _N> & board) {
	auto output = std::map<_Ty, std::vector<AbstractCell<_Ty, _N>*>>();
	for (_Ty m = 1; m <= _N; m++) {
		for (_Ty i = 0; i < _N * _N; ++i) {
			if (board.cell(i).containsMark(m))
				output[m].push_back(&board.cell(i));
		}
	}
	return output;
};

SUDOKU_TEMPLATE
std::vector<_Ty> markingIntersection(std::vector<AbstractCell<_Ty, _N>*> & arg0) {
	auto output = std::vector<_Ty>();
	auto start = std::find_if(arg0.begin(), arg0.end(), [](AbstractCell<_Ty, _N> * cell) { // Finding the first empty cell
		return cell->isEmpty();
	});
	if (start == arg0.end())
		return output; // marking intersections don't matter if all the cells are not empty

	AbstractCell<_Ty, _N> * cell_ptr = *start;

	// iterating through each non-empty mark (using the operator[]) in the very first cell in arg0 and checking if that marking is present in all other cells
	for (_Ty i = 0, mark = cell_ptr->operator[](0); mark != EMPTY_VALUE; ++i, mark = cell_ptr->operator[](i)) {
		for (auto it = start; it != arg0.end(); ++it) {
			if (!(*it)->isEmpty())
				continue;
			if ((*it)->containsMark(mark))
				output.push_back(mark);
		}
	}
	return output;
}

namespace SudokuAlgorithms {
	// https://www.sudokuwiki.org/Strategy_Families

	SUDOKU_TEMPLATE
	bool removeMarkings(AbstractUnit<_Ty, _N> & unit) {
		bool mutated = false;
		for (_Ty i = 0; i < _N; ++i) {
			AbstractCell<_Ty, _N> & cell = unit.cell(i);
			if (cell.isEmpty())
				continue;

			for (_Ty k = 0; k < _N; ++k) {
				AbstractCell<_Ty, _N> & cell2 = unit.cell(k);
				if (!cell2.isEmpty())
					continue;

				mutated |= cell2.unmark(cell.getValue());
			}
		}
		return mutated;
	};

	SUDOKU_TEMPLATE
	bool removeMarkings(AbstractBoard<_Ty, _N> & board) {
		bool mutated = false;
		for (_Ty i = 0; i < _N; ++i) {
			mutated |= removeMarkings(board.row(i));
			mutated |= removeMarkings(board.col(i));
			mutated |= removeMarkings(board.box(i));
		}
		return mutated;
	};

	SUDOKU_TEMPLATE
	bool nakedSingle(AbstractUnit<_Ty, _N> & unit) {
		/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
		If found, that cell's value becomes that mark.

		This algorithm is just a slightly worse version of the hiddenSingle algorithm. While the hidden single algorithm does do the same thing as this (and more),
		this algorithm does it's job slightly faster than the hiddenSingle algorithm, so there is a trade-off there.
		*/
		bool mutated = false;
		for (_Ty i = 0; i < _N; ++i) {
			AbstractCell<_Ty, _N> & cell = unit.cell(i);
			if (!cell.isEmpty())
				continue;

			if (cell[1] == EMPTY_VALUE) // This means that there is only 1 marking present
				mutated |= cell.setValue(cell[0]);
		}
		return mutated;
	};

	SUDOKU_TEMPLATE
	bool nakedSingle(AbstractBoard<_Ty, _N> & board) {
		/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
		If found, that cell's value becomes that mark.

		This algorithm is just a slightly worse version of the hiddenSingle algorithm. While the hidden single algorithm does do the same thing as this (and more),
		this algorithm does it's job slightly faster than the hiddenSingle algorithm, so there is a trade-off there.
		*/
		bool mutated = false;
		for (_Ty i = 0; i < _N * _N; ++i) {
			AbstractCell<_Ty, _N> cell = board.cell(i);
			if (!cell.isEmpty())
				continue;

			if (cell[1] == EMPTY_VALUE)
				mutated |= cell.setValue(cell[0]);
		}
		return mutated;
	};
	
	SUDOKU_TEMPLATE
	bool hiddenSingle(AbstractUnit<_Ty, _N> & unit) {
		/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
		If found, that cell's value becomes that mark.
		*/
		bool mutated = false;
		for (_Ty m = 1; m <= _N; ++m) {
			auto func = [&m](AbstractCell<_Ty, _N> & cell) {
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
	
	SUDOKU_TEMPLATE
	bool hiddenSingle(AbstractBoard<_Ty, _N> & board) {
		bool mutated = false;
		for (_Ty i = 0; i < _N; ++i) {
			mutated |= board.row(i);
			mutated |= board.col(i);
			mutated |= board.box(i);
		}
		return mutated;
	};

	SUDOKU_TEMPLATE
	bool hiddenSequence(AbstractUnit<_Ty, _N> & unit) {
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
				if (it->second[0]->setValue(it->first))
					return true;
			}
			if (it->second.size() < 2)
				continue;

			auto sharedMarkings = markingIntersection(it->second);
			if (sharedMarkings.size() <= 0)
				throw "Something when wrong with markingIntersection()";
			else if (sharedMarkings.size() != markingMap[it->first].size())
				continue;

			for (AbstractCell<_Ty, _N> * cell : it->second) {
				for (_Ty i = 0, mark = cell->operator[](i); i < _N && mark != EMPTY_VALUE; ++i, mark = cell->operator[](i)) {
					if (std::find(sharedMarkings.begin(), sharedMarkings.end(), mark) == sharedMarkings.end()) {
						mutated |= cell->unmark(mark);
					}
				}
			}
			return mutated;
		}
		return mutated;
	};
	
	SUDOKU_TEMPLATE
	bool hiddenSequence(AbstractBoard<_Ty, _N> & board) {
		bool mutated = false;
		for (_Ty i = 0; i < _N; i++) {
			mutated |= hiddenSequence(board.box(i));
			mutated |= hiddenSequence(board.row(i));
			mutated |= hiddenSequence(board.col(i));
		}
		return mutated;
	};
	
	SUDOKU_TEMPLATE
	bool intersectionRemoval(AbstractBoard<_Ty, _N> & board) {
		/* https://www.sudokuwiki.org/Intersection_Removal
		Assumptions:
		n = the size of the user supplied board.

		Conditions to satisfy:
		1. Looking for at least 2 empty unique cells.
		2. These cells must all share 1 marking.
		3. The cells must all be in the same box and must meet 1 of the following sub-conditions:
			3a. The cells must be aligned on the same row and they must be the only cells with the shared marking in the row or box.
			3b. The cells must be aligned on the same column and they must be the only cells with the shared marking in the column or box.
		If all conditions are satisfied:
		- the action varies depending on which sub-conditions are satisfied
		*/

		bool mutated = false;
		auto markingMap = mapCellsToMarks(board); // satisfies condition 2

		for (auto it = markingMap.begin(); it != markingMap.end(); ++it) {
			if (it->second.size() <= 0)
				throw "Something went wrong with mapCellsToMarks()";
			if (it->second.size() == 1) { // We found a naked/hidden single!
				mutated |= it->second[0].setValue(it->first);
				return true;
			}

			// Now we have to filter the collection of cells to cells that are in the same box, but we need to do this for each box
			for (_Ty bx = 0; bx < _N; ++bx) {
				// Getting cells that are all in the same box
				std::vector<AbstractCell<_Ty, _N>> cellsInBox = {};
				std::for_each(it->second.begin(), it->second.end(), [&bx, &cellsInBox](AbstractCell<_Ty, _N> & cell) {
					if (cell.getBoxIndex() == bx)
						cellsInBox.push_back(cell);
				});

				if (cellsInBox.size() > AbstractCell<_Ty, _N>::ROOT_N) { // This means they cannot all be aligned on a row or column inside the box (but some may be the only ones in the row)
					// Alright, this one is going to get kinda crazy.
					// We need to iterate thru each possible row and column the box encompasses and check if the only cells containing the marking are inside our box

					auto pred = [&it, &bx](AbstractCell<_Ty, _N> & cell) {
						return cell.isEmpty() && cell.getBoxIndex() != bx && cell.containsMark(it->first);
					};
					auto pred2 = [&it, &bx](AbstractCell<_Ty, _N> & cell) {
						return cell.isEmpty() && cell.getBoxIndex() == bx && cell.containsMark(it->first);
					};
					auto func = [&cellsInBox, &mutated](AbstractCell<_Ty, _N> & cell) {
						if (!cell.isEmpty() || std::find(cellsInBox.begin(), cellsInBox.end(), cell) != cellsInBox.end())
							return;
						mutated |= cell.unmark(it->first);
					};

					// Iterating thru the rows first
					for (_Ty r = bx / AbstractCell<_Ty, _N>::ROOT_N; r < r + AbstractCell<_Ty, _N>::ROOT_N; ++r) {
						if (std::any_of(board.row(r).begin(), board.row(r).end(), pred))
							continue;
						// We need to make sure there is at least 1 cell in this row with the marking
						auto n = std::count_if(cellsInBox.begin(), cellsInBox.end(), pred2);
						if (n == 1) {
							for (_Ty x = 0; x < cellsInBox.size(); ++x) {
								if (cellsInBox[x].getRowIndex() == r) {
									cellsInBox[x].setValue(it->first);
									return true;
								}
							}
						} else if (n > 1) {
							std::for_each(board.box(bx).begin(), board.box(bx).end(), func);
						}
					}

					// Iterating thru the cols next
					for (_Ty c = bx % AbstractCell<_Ty, _N>::ROOT_N; c < c + AbstractCell<_Ty, _N>::ROOT_N; ++c) {
						if (std::any_of(board.col(c).begin(), board.col(c).end(), pred))
							continue;
						// We need to make sure there is at least 1 cell in this row with the marking
						auto n = std::count_if(cellsInBox.begin(), cellsInBox.end(), pred2);
						if (n == 1) {
							for (_Ty x = 0; x < cellsInBox.size(); ++x) {
								if (cellsInBox[x].getColIndex() == x) {
									cellsInBox[x].setValue(it->first);
									return true;
								}
							}
						} else if (n > 1) {
							std::for_each(board.box(bx).begin(), board.box(bx).end(), func);
						}
					}
				} else if (cellsInBox.size() >= 2) { // This means they could all be aligned on a row or column inside the box
					// Checking if the cells are aligned on any rows or cols

					bool alignedOnRow = std::all_of(cellsInBox.cbegin(), cellsInBox.cend(), [&cellsInBox](const AbstractCell<_Ty, _N> & cell) {
						return cell.getRowIndex() == cellsInBox[0].getRowIndex();
					});
					bool alignedOnCol = std::all_of(cellsInBox.cbegin(), cellsInBox.cend(), [&cellsInBox](const AbstractCell<_Ty, _N> & cell) {
						return cell.getColIndex() == cellsInBox[0].getColIndex();
					});

					if (alignedOnRow || alignedOnCol) { // Condition 3a or 3b are satisfied

						_Ty z = alignedOnRow ? cellsInBox[0].getRowIndex() : cellsInBox[0].getColIndex();
						AbstractUnit<_Ty, _N> u = alignedOnRow ? board.row(z) : board.col(z);

						std::for_each(u.begin(), u.end(), [&cellsInBox, &mutated](AbstractCell<_Ty, _N> & cell) {
							if (!cell.isEmpty() || std::find(cellsInBox.begin(), cellsInBox.end(), cell) != cellsInBox.end())
								return;
							mutated |= cell.unmark(it->first);
						});
					}
				}
			}
		}
		return mutated;
	};
	
	SUDOKU_TEMPLATE
	bool xwing(AbstractBoard<_Ty, _N> & board) {
		/* https://www.sudokuwiki.org/X_Wing_Strategy
		Conditions to satisfy:
		1. Looking for exactly 4 unique cells that are four corners of an imaginary rectangle.
		2. These corners must be in their own boxes.
		3. These cells must each share a singular marking with each other. (ie all 4 cells must be marked with a 7)
		4. These cells must meet either, but not both (b/c there would be nothing to change), of these two conditions:
			4a. Looking at the cells' rows, both rows of corners only have those 2 cells as possible candidates for the marking in question.
			4b. Looking at the cells' columns, both columns of corners only have those 2 cells as possible candidates for the marking in question.

		- If all conditions are met, then you do one of two things depending on which of the previous conditions were met:
			- Looking at the cells' columns, remove the marking from all other cells in the column.
			- Looking at the cells' rows, remove the marking from all other cells in the row.

		Applies this algorithm to the first xwing it finds.
		*/

		auto markingMap = mapCellsToMarks(board); // Satisfies condition 3 b/c we'll be iterating thru the pairs
		for (auto it = markingMap.begin(); it != markingMap.end(); ++it) {
			if (it->second.size() < 4) // Partially satisfies the condition 1's quantity requirement
				continue;

			// We're gonna try to find the diagonal corner b/c it'd cut down on the amount of crap we have to iterate through
			/* Here are some diagrams to visualize the variable names to the positions on the board.
			cell1---------cell2        cell3---------cell4        cell4---------cell3        cell2---------cell1
			  |             |            |             |            |             |            |             |
			  |             |     or     |             |     or     |             |     or     |             |
			  |             |            |             |            |             |            |             |
			cell3---------cell4        cell1---------cell2        cell2---------cell1        cell4---------cell3
			*/

			for (AbstractCell<_Ty, _N> & cell1 : it->second) {
				_Ty boxIndex1 = cell1.getBoxIndex();
				_Ty boardBoxRowIndex1 = boxIndex1 / AbstractCell<_Ty, _N>::ROOT_N;
				_Ty boardBoxColIndex1 = boxIndex1 % AbstractCell<_Ty, _N>::ROOT_N;

				for (AbstractCell<_Ty, _N> & cell4 : it->second) {
					if (cell1 == cell4)
						continue; // Skipping duplicates (cells must be unique)
					_Ty boxIndex4 = getBoxIndex(cell4.getRowIndex(), cell4.getColIndex(), AbstractCell<_Ty, _N>::ROOT_N);
					_Ty boardBoxRowIndex4 = boxIndex4 / AbstractCell<_Ty, _N>::ROOT_N;
					_Ty boardBoxColIndex4 = boxIndex4 % AbstractCell<_Ty, _N>::ROOT_N;

					// Now we'll check if the two indices are either in the same row of boxes or in the same column of boxes, which will satisfy condition 2.
					if (boardBoxRowIndex1 == boardBoxRowIndex4 || boardBoxColIndex1 == boardBoxColIndex4)
						continue; // Skipping if the condition isn't met


					// Now we know that cell1 and cell4 are in fact in boxes that are diagonal from each other.
					// Now we're going to be checking if the other 2 corners share the same mark, thus satisfying the rest of condition 1 and condition 3.

					AbstractCell<_Ty, _N> & cell2 = board.cell(cell1.getRowIndex(), cell4.getColIndex());
					AbstractCell<_Ty, _N> & cell3 = board.cell(cell4.getRowIndex(), cell1.getColIndex());

					if (!cell2.containsMark(it->first) || !cell3.containsMark(it->first))
						continue; // Skipping if the condition isn't met


					/* Now we're going to try to satisfy condition 4.

					We'll do this by counting how many markings are in the cell's rows and cols and if the row count xor the col count is exactly 2,
					then we have an xwing and condition 4 will be satisfied.
					*/

					auto rowCount1 = board.row(cell1.getRowIndex()).countCellsMarkedWith(it->first);
					auto colCount1 = board.col(cell1.getColIndex()).countCellsMarkedWith(it->first);

					auto rowCount4 = board.row(cell4.getRowIndex()).countCellsMarkedWith(it->first);
					auto colCount4 = board.col(cell4.getColIndex()).countCellsMarkedWith(it->first);

					auto func1 = [&cell1, &cell3, &it](AbstractCell<_Ty, _N> & c) { // Function we'll use for removing markings
						if (c == cell1 || c == cell3)
							return;
						c.unmark(it->first);
					};

					auto func4 = [&cell4, &cell2, &it](AbstractCell<_Ty, _N> & c) { // Function we'll use for removing markings
						if (c == cell4 || c == cell2)
							return;
						c.unmark(it->first);
					};

					if ((rowCount1 == 2 && rowCount4 == 2) && (colCount1 > 2 || colCount4 > 2)) { // We added this other condition that ensures we actually need to do anything
						// Condition 4a satisfied. Now we'll remove the marking from all the cells except the four cells
						auto col1 = board.col(cell1.getColIndex());
						auto col4 = board.col(cell4.getColIndex());
						std::for_each(col1.begin(), col1.end(), func1);
						std::for_each(col4.begin(), col4.end(), func4);
						return true; // returning true b/c we have mutated the board
					} else if ((colCount1 == 2 && colCount4 == 2) && (rowCount1 > 2 || rowCount4 > 2)) {
						// Condition 4b satisfied. Now we'll remove the marking from all the cells except the four cells
						auto row1 = board.row(cell1.getRowIndex());
						auto row4 = board.row(cell4.getRowIndex());
						std::for_each(row1.begin(), row1.end(), func1);
						std::for_each(row4.begin(), row4.end(), func4);
						return true; // returning true b/c we have mutated the board
					}
				}
			}
		}
		return false;
	};
	
	SUDOKU_TEMPLATE
	bool ywing(AbstractBoard<_Ty, _N> & board) {
		/* https://www.sudokuwiki.org/Y_Wing_Strategy
		Conditions to satisfy:
		1. Looking for exactly 3 unique cells laid out like one of the following:
			1a. 2 of the 3 cells share the same box, but those 2 cells cannot share the same row or column and the last cell must share a row xor column with 1 of the 2 cells
			1b. All 3 cells are in their own boxes, each cell is a corner of a right triangle.
		2. For this condition, we'll use arbitrary cells named A, B, and C. We'll also use arbitrary markings a,b, and c to make this clearer:
			2a. Cells A and B must share markings a and b

		If conditions are met:
		- do stuff
		*/

		return false;
	};
};

