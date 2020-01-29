#pragma once
#include <iostream>
#include <array>
#include <algorithm>
#include "Cell.h"
#include "Utils.h"

template <
	class _Ty, // the data type to use for the values and the markings (for the cells)
	_Ty _N, // how many cells in the unit
	typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value> // requiring that _Ty is an unsigned number
>
class Unit {
	using BoxUnit = std::array<Cell<_Ty, _N>, SudokuUtils::isqrt(_N)>;

	std::array< Cell<_Ty, _N>, _N > cells;
	std::array< BoxUnit, _N >     boxRows;
	std::array< BoxUnit, _N >     boxCols;
	_Ty                             index; // index for the row/col/box (depending on what the unit is)
public:
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

			auto row = cells[i].getRowIndex();
			auto col = cells[i].getColIndex();

			boxRows[row][col] = Cell<_Ty, _N>(cells[i]); // Shallow copying
			boxCols[col][row] = Cell<_Ty, _N>(cells[i]); // Shallow copying
		}
	}

	Unit(std::array<Cell<_Ty, _N>, _N> && cells)
		: Unit<_Ty, _N>(cells.begin(), cells.end()) {};

	// This should only be used as a place holder for vectors and arrays
	Unit() {
		cells = std::array< Cell<_Ty, _N>, _N >();
		boxRows = std::array< BoxUnit, _N >();
		boxCols = std::array< BoxUnit, _N >();
	};

	~Unit() {
		// RAII save us.
	};

	// ===============================================================================
	//								Member Functions
	// ===============================================================================

	Cell<_Ty, _N> &       operator[](_Ty i) { return cells[i]; }
	Cell<_Ty, _N> &       cell(_Ty i) {
		return cells[i];
	}
	const Cell<_Ty, _N> & cell(_Ty i) const {
		return cells[i];
	}
	Cell<_Ty, _N> &       cell(_Ty r, _Ty c) {
		return boxRows[r][c];
	}
	const Cell<_Ty, _N> & cell(_Ty r, _Ty c) const {
		return boxRows[r][c];
	}

	_Ty countCellsMarkedWith(_Ty marking) const {
		return std::count_if(cells.cbegin(), cells.cend(), [&marking](const Cell<_Ty, _N> & cell) {
			return cell.containsMark(marking);
		});
	}





};