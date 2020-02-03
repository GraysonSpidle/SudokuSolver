#pragma once
#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include "AbstractUnit.h"
#include "Cell.h"
#include "Utils.h"

SUDOKU_TEMPLATE
class Unit : public AbstractUnit<_Ty, _N> {
public:
	static constexpr const _Ty ROOT_N = SudokuUtils::isqrt(_N);
	using BoxUnit = std::array<Cell<_Ty, _N>, ROOT_N>; // A row or column inside a box

	std::array< Cell<_Ty, _N>, _N >    cells;
	std::array< BoxUnit, ROOT_N >     boxRows;
	std::array< BoxUnit, ROOT_N >     boxCols;

	// ===============================================================================
	//							 Constructors / Destructors
	// ===============================================================================

	// The actual constructor
	template <class _It>
	Unit(_It begin, _It end) {
		cells = std::array< Cell<_Ty, _N>, _N >();
		boxRows = std::array< BoxUnit, _N >();
		boxCols = std::array< BoxUnit, _N >();

		auto it = begin;
		for (_Ty i = 0; i < _N; ++it, ++i) {
			assert(it != end);
			cells[i] = *it; // This should be fine since *it should return a Cell<_Ty, _N>& and the Cell<_Ty, _N>(_Ty) constructor is explicit

			_Ty row = cells[i].getRowIndex();
			_Ty col = cells[i].getColIndex();

			boxRows[row][col] = Cell<_Ty, _N>(cells[i]); // Shallow copying
			boxCols[col][row] = Cell<_Ty, _N>(cells[i]); // Shallow copying
		}


	}

	Unit(std::array<_Ty, _N> && rawValues)
		: Unit<_Ty, _N>(rawValues.begin(), rawValues.end()) {};

	// This should only be used as a place holder for vectors and arrays
	Unit() {
		cells = std::array< Cell<_Ty, _N>, _N >(); // This time around, we'll treat this as if it were ordered
		boxRows = std::array< BoxUnit, ROOT_N >();
		boxCols = std::array< BoxUnit, ROOT_N >();

		for (_Ty i = 0; i < _N; ++i) {
			_Ty cellRow = i / ROOT_N; // this should do floor division
			_Ty cellCol = i % ROOT_N; // this should work since they're both not a double or float b/c those data types cannot be unsigned

			Cell<_Ty, _N> cell = cells[i];
			cell.setRowIndex(cellRow);
			cell.setColIndex(cellCol);
			
			boxRows[cellRow][cellCol] = Cell<_Ty, _N>(cell); // Shallow copying
			boxCols[cellCol][cellRow] = Cell<_Ty, _N>(cell); // Shallow copying
		}
	};

	~Unit() {
		// RAII save us.
	};

	// ===============================================================================
	//								Member Functions
	// ===============================================================================

	Cell<_Ty, _N> &       operator[](_Ty i) override { return cells[i]; }
	Cell<_Ty, _N> &       cell(_Ty i) override {
		return cells[i];
	}

	Cell<_Ty, _N> & cell(_Ty i) const {
		return cells[i];
	}
	Cell<_Ty, _N> & cell(_Ty r, _Ty c) override {
		for (auto it = cells.begin(); it != cells.end(); ++it) {
			if (it->getRowIndex() == r && it->getColIndex() == c)
				return *it;
		}
		throw "Couldn't find specified cell";
		return cells[0];
	}
	Cell<_Ty, _N> & cell(_Ty r, _Ty c) const {
		return boxRows[r][c];
	}

	_Ty countCellsMarkedWith(_Ty marking) const override {
		return std::count_if(cells.cbegin(), cells.cend(), [&marking](const Cell<_Ty, _N> & cell) {
			return cell.containsMark(marking);
		});
	}

	std::string toString() {
		std::string s = "";
		for (_Ty i = 0; i < _N; ++i) {
			s = s.append(cells[i].toString());
		}
		return s;
	}





};