#pragma once
#include <array>
#include "AbstractBoard.h"
#include "Unit.h"
#include "Cell.h"
#include "Utils.h"

SUDOKU_TEMPLATE
class Board : public AbstractBoard<_Ty, _N> {
	std::array<Cell<_Ty, _N>, _N * _N> cells;
	std::array<Unit<_Ty, _N>, _N>       rows;
	std::array<Unit<_Ty, _N>, _N>       cols;
	std::array<Unit<_Ty, _N>, _N>      boxes;
public:
	Board(std::array<_Ty, _N * _N> & rawValues) {
		cells = std::array< Cell<_Ty, _N>, _N * _N >();
		rows = std::array< Unit<_Ty, _N>, _N >();
		cols = std::array< Unit<_Ty, _N>, _N >();
		boxes = std::array< Unit<_Ty, _N>, _N >();

		for (_Ty i = 0; i < _N * _N; ++i) { // i is the cell index for the general array of cells that we don't expect to be in any particular order
			Cell<_Ty, _N> & cell = cells[i];
			_Ty cellRow = i / _N; // this should do floor division
			_Ty cellCol = i % _N; // this should work since they're both not a double or float b/c those data types cannot be unsigned
			_Ty cellBox = SudokuUtils::calculateBoxIndex(cellRow, cellCol, _N);
			std::pair<_Ty, _Ty> localBoxCoords = SudokuUtils::calculateLocalBoxCoords(cellRow, cellCol, _N);

			_Ty val = rawValues[i];
			
			cell.setValue(val);
			cell.setRowIndex(cellRow);
			cell.setColIndex(cellCol);

			rows[cellRow][cellCol] = Cell<_Ty, _N>(cell); // Shallow copying
			cols[cellCol][cellRow] = Cell<_Ty, _N>(cell); // Shallow copying

			for (_Ty k = 0; k < _N; ++k) {


				if (boxes[cellBox].cell(k).getRowIndex() == localBoxCoords.first && boxes[cellBox].cell(k).getColIndex() == localBoxCoords.second) {
					boxes[cellBox].cell(k) = Cell<_Ty, _N>(cell); // Shallow copying
					break;
				}
			}
		}
	}

	~Board() {
		// RAII to the rescue
	}

	AbstractCell<_Ty, _N> & cell(_Ty i)        override { return cells[i]; }
	AbstractCell<_Ty, _N> & cell(_Ty r, _Ty c) override { return rows[r][c]; }
	AbstractUnit<_Ty, _N> & row(_Ty i)         override { return rows[i]; }
	AbstractUnit<_Ty, _N> & col(_Ty i)         override { return cols[i]; }
	AbstractUnit<_Ty, _N> & box(_Ty i)         override { return boxes[i]; }
};
