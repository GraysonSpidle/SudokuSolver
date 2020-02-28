#include "pch.h"
#include "Algorithms.h"


SUDOKU_TEMPLATE_EXTRA
HSGraph& createHSGraph(const MarkingMap& markingMap) {
	using E = Edge<CellPtr, _Ty>;
	std::set<E> edges{};

	for (auto it = markingMap.begin(); it != markingMap.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			for (auto it3 = it2; it3 != it->second.end(); ++it3) {
				if (it3 == it2)
					continue;
				edges.insert(E(*it2, *it3, it->first));
			}
		}
	}

	return HSGraph(edges);
}

template <typename _Ty>
std::vector<std::vector<_Ty>> & generatePermutations(const std::set<_Ty> & s) {
	if (n > s.size())
		throw "n cannot be bigger than the set you're generating a hiddenSequences out of";


	std::vector<std::vector<_Ty, _Ty>> output = {};
	for (std::size_t i = 0; i < s.size() - 1; ++i) {
		output.push_back(std::pair(s[i], s[i + 1]));
	}
	output.push_back(std::pair(s[0], s[s.size() - 1]));

	return output;
}

SUDOKU_TEMPLATE_EXTRA
MarkingMap& mapCellsToMarks(AbstractUnit<_Ty, _N> & unit) {
	MarkingMap output = {};
	for (_Ty m = 1; m <= _N; m++) {
		for (_Ty i = 0; i < _N; ++i) {
			if (unit.cell(i).containsMark(m))
				output[m].insert(&unit.cell(i));
		}
	}
	return output;
};

SUDOKU_TEMPLATE_EXTRA
MarkingMap& mapCellsToMarks(AbstractBoard<_Ty, _N> & board) {
	MarkingMap output = {};
	for (_Ty m = 1; m <= _N; m++) {
		for (_Ty i = 0; i < _N * _N; ++i) {
			if (board.cell(i).containsMark(m))
				output[m].insert(&board.cell(i));
		}
	}
	return output;
};

SUDOKU_TEMPLATE_EXTRA
std::vector<_Ty> markingIntersection(std::vector<CellPtr> & arg0) {
	auto output = std::vector<_Ty>();
	auto start = std::find_if(arg0.begin(), arg0.end(), [](CellPtr& cell) { // Finding the first empty cell
		return cell->isEmpty();
	});
	if (start == arg0.end())
		return output; // marking intersections don't matter if all the cells are not empty

	CellPtr cell_ptr = { *start };

	// iterating through each non-empty mark (using the operator[]) in the very first cell in arg0 and checking if that marking is present in all other cells
	for (_Ty i = 0, mark = cell_ptr->operator[](0); mark != 0; ++i, mark = cell_ptr->operator[](i)) {
		for (auto it = start; it != arg0.end(); ++it) {
			if (!(*it)->isEmpty())
				continue;
			if ((*it)->containsMark(mark))
				output.push_back(mark);
		}
	}
	return output;
}

SUDOKU_TEMPLATE_EXTRA
typename HSGraph::VertexIterator findNextHSCandidate(typename HSGraph::VertexIterator& begin,
	typename HSGraph::VertexIterator& end,
	const MarkingMap& markingMap,
	const std::set< _Ty >& otherMarkings) {
	
	std::find_if(begin, end, [&markingMap, &otherMarkings](CellPtr& cell) {
		for (_Ty i = 0, m = (*cell)[i]; m != 0; m = (*cell)[++i]) {

		}
		markingMap.
	});

	

	return end;
}






SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::removeMarkings(AbstractUnit<_Ty, _N> & unit) {
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

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::removeMarkings(AbstractBoard<_Ty, _N> & board) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		mutated |= removeMarkings(board.row(i));
		mutated |= removeMarkings(board.col(i));
		mutated |= removeMarkings(board.box(i));
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::nakedSingle(AbstractUnit<_Ty, _N> & unit) {
	/* Find a cell in a row or col or box that happens to be the only cell marked with a certain mark.
	If found, that cell's value becomes that mark.

	This algorithm is just a slightly worse version of the hiddenSingle algorithm. While the hidden single algorithm does do the same thing as this (and more),
	this algorithm does its job slightly faster than the hiddenSingle algorithm, so there is a trade-off there.
	*/
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		AbstractCell<_Ty, _N> & cell = unit.cell(i);
		if (!cell.isEmpty())
			continue;

		if (cell[1] == 0) // This means there is only 1 marking present
			mutated |= cell.setValue(cell[0]);
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::nakedSingle(AbstractBoard<_Ty, _N> & board) {
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

		if (cell[1] == 0)
			mutated |= cell.setValue(cell[0]);
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::hiddenSingle(AbstractUnit<_Ty, _N> & unit) {
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

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::hiddenSingle(AbstractBoard<_Ty, _N> & board) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		mutated |= board.row(i);
		mutated |= board.col(i);
		mutated |= board.box(i);
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool hiddenSequence(_Ty size,
	MarkingMap& markingMap,
	HSGraph& graph) {

	std::vector<CellPtr> output {};


	return false;
}

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::hiddenSequence(AbstractUnit<_Ty, _N> & unit) {
	/* https://www.sudokuwiki.org/Hidden_Candidates
	Tasked with searching for a hidden sequence of size n:
	- We're looking for a cycle which contains 4 distinct vertices and 4 edges with distinct weights.
	- The weights of the edges represent the marking that links the two vertices (cells) together
	- Each vertex in the cycle must be used 2 times

	- Don't do hidden sequences that are > ROOT_N
	- Disregard mapped sets that are > ROOT_N
	- Each marking must appear atleast twice in unique mapped sets

	*/

	using CellPair = std::pair<CellPtr, CellPtr>;
	const _Ty ROOT_N = SudokuUtils::isqrt(_N);

	bool mutated = false;
	MarkingMap markingMap = mapCellsToMarks(unit);
	HSGraph graph = createHSGraph(markingMap);
	for (auto it = markingMap.begin(); it != markingMap.end(); ++it) { // Removing the mapped sets larger than ROOT_N in size
		if (it->second.size() > ROOT_N)
			markingMap.erase(it->first);
	}

	std::vector<CellPtr> l = {};
	std::vector<_Ty> markings = {};
	for (_Ty size = 2; size <= ROOT_N; ++size, markings.clear(), l.clear()) {
		for (_Ty i = 0; i < size; ++i) {
			// Iterating through the pairs in markingMap
			for (auto it = markingMap.begin(); it != markingMap.end(); ++it) {
				_Ty marking = it->first;
				std::set<CellPtr> cells = it->second;
				if (cells.size() > size)
					continue;

				markings.push_back




			}

		}
	}


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
			for (_Ty i = 0, mark = cell->operator[](i); i < _N && mark != SUDOKU_EMPTY_VALUE; ++i, mark = cell->operator[](i)) {
				if (std::find(sharedMarkings.begin(), sharedMarkings.end(), mark) == sharedMarkings.end()) {
					mutated |= cell->unmark(mark);
				}
			}
		}
		return mutated;
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::hiddenSequence(AbstractBoard<_Ty, _N> & board) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; i++) {
		mutated |= hiddenSequence(board.box(i));
		mutated |= hiddenSequence(board.row(i));
		mutated |= hiddenSequence(board.col(i));
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::intersectionRemoval(AbstractBoard<_Ty, _N> & board) {
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
	MarkingMap markingMap = mapCellsToMarks(board); // satisfies condition 2

	for (auto it = markingMap.begin(); it != markingMap.end(); ++it) {
		if (it->second.size() <= 0)
			throw "Something went wrong with mapCellsToMarks()";
		if (it->second.size() == 1) { // We found a naked/hidden single!
			mutated |= it->second[0]->setValue(it->first);
			return true;
		}

		// Now we have to filter the collection of cells to cells that are in the same box, but we need to do this for each box
		for (_Ty bx = 0; bx < _N; ++bx) {
			// Getting cells that are all in the same box
			std::vector<AbstractCell<_Ty, _N>*> cellsInBox = {};
			std::for_each(it->second.begin(), it->second.end(), [&bx, &cellsInBox](AbstractCell<_Ty, _N> * cell) {
				if (cell->getBoxIndex() == bx)
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
				auto func = [&cellsInBox, &mutated, &it](AbstractCell<_Ty, _N> & cell) {
					if (!cell.isEmpty() || std::find(cellsInBox.begin(), cellsInBox.end(), cell) != cellsInBox.end())
						return;
					mutated |= cell.unmark(it->first);
				};

				// Iterating thru the rows first
				for (_Ty r = bx / AbstractCell<_Ty, _N>::ROOT_N; r < r + AbstractCell<_Ty, _N>::ROOT_N; ++r) {
					if (board.row(r).anyOf(pred))
						continue;
					// We need to make sure there is at least 1 cell in this row with the marking
					auto n = std::count_if(cellsInBox.begin(), cellsInBox.end(), pred2);
					if (n == 1) {
						for (_Ty x = 0; x < cellsInBox.size(); ++x) {
							if (cellsInBox[x]->getRowIndex() == r) {
								cellsInBox[x]->setValue(it->first);
								return true;
							}
						}
					}
					else if (n > 1) {
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
							if (cellsInBox[x]->getColIndex() == x) {
								cellsInBox[x]->setValue(it->first);
								return true;
							}
						}
					}
					else if (n > 1) {
						std::for_each(board.box(bx).begin(), board.box(bx).end(), func);
					}
				}
			}
			else if (cellsInBox.size() >= 2) { // This means they could all be aligned on a row or column inside the box
			 // Checking if the cells are aligned on any rows or cols

				bool alignedOnRow = std::all_of(cellsInBox.cbegin(), cellsInBox.cend(), [&cellsInBox](const AbstractCell<_Ty, _N> * cell) {
					return cell->getRowIndex() == cellsInBox[0]->getRowIndex();
				});
				bool alignedOnCol = std::all_of(cellsInBox.cbegin(), cellsInBox.cend(), [&cellsInBox](const AbstractCell<_Ty, _N> * cell) {
					return cell->getColIndex() == cellsInBox[0]->getColIndex();
				});

				if (alignedOnRow || alignedOnCol) { // Condition 3a or 3b are satisfied

					_Ty z = alignedOnRow ? cellsInBox[0]->getRowIndex() : cellsInBox[0]->getColIndex();
					AbstractUnit<_Ty, _N> u = alignedOnRow ? board.row(z) : board.col(z);

					std::for_each(u.begin(), u.end(), [&cellsInBox, &mutated](AbstractCell<_Ty, _N> * cell) {
						if (!cell->isEmpty() || std::find(cellsInBox.begin(), cellsInBox.end(), cell) != cellsInBox.end())
							return;
						mutated |= cell->unmark(it->first);
					});
				}
			}
		}
	}
	return mutated;
};

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::xwing(AbstractBoard<_Ty, _N> & board) {
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

	// TODO fix this

	MarkingMap markingMap = mapCellsToMarks(board); // Satisfies condition 3 b/c we'll be iterating thru the pairs
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
				}
				else if ((colCount1 == 2 && colCount4 == 2) && (rowCount1 > 2 || rowCount4 > 2)) {
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

SUDOKU_TEMPLATE_EXTRA
bool SudokuAlgorithms::ywing(AbstractBoard<_Ty, _N> & board) {
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