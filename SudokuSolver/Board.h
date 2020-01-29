#pragma once
#include <array>
#include "Unit.h"
#include "Cell.h"
#include "Utils.h"

template <
	class _Ty, // the data type to use for the values and the markings (for the cells)
	_Ty _N, // how many cells in 1 row/col/box of the board
	typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value> // requiring that _Ty is an unsigned number
>
class Board {
	std::array<Cell<_Ty, _N>, _N * _N> cells;
	std::array<Unit<_Ty, _N>, _N>       rows;
	std::array<Unit<_Ty, _N>, _N>       cols;
	std::array<Unit<_Ty, _N>, _N>      boxes;

	static constexpr const _Ty ROOT_N = SudokuUtils::isqrt(_N);

public:
	// ===============================================================================
	//							 Constructors / Destructors
	// ===============================================================================

	Board(std::array<_Ty, _N * _N> & raw) {
		cells = std::array< Cell<_Ty, _N>, (_N * _N) >();
		rows = std::array< Unit<_Ty, _N>, _N >();
		cols = std::array< Unit<_Ty, _N>, _N >();
		boxes = std::array< Unit<_Ty, _N>, _N >();

		for (_Ty i = 0; i < _N * _N; ++i) {
			_Ty cellRow = i / _N; // this should do floor division
			_Ty cellCol = i % _N; // this should work since they're both not a double or float b/c those data types cannot be unsigned
			
			cells[i]                         = Cell<_Ty, _N>(cellRow, cellCol, raw[i]);
			rows[cellRow][cellCol]           = cells[i]; // Shallow copying
			cols[cellCol][cellRow]           = cells[i]; // Shallow copying
			boxes[cells[i].getBoxIndex()][i] = cells[i]; // Shallow copying
		}

	};
	~Board() {
		// RAII hopefully doesn't fail us here
	};
	
	// ===============================================================================
	//								Member Functions
	// ===============================================================================

	Cell<_Ty, _N> & cell(_Ty i) { return cells[i]; }
	Cell<_Ty, _N> & cell(_Ty r, _Ty c) { return rows[r][c]; }
	Unit<_Ty, _N> & row(_Ty i) { return rows[i]; }
	Unit<_Ty, _N> & col(_Ty i) { return cols[i]; }
	Unit<_Ty, _N> & box(_Ty i) { return boxes[i]; }
	Unit<_Ty, _N> & box(_Ty r, _Ty c) { return  box(ROOT_N * (r / ROOT_N) + (c / ROOT_N)); }

};
