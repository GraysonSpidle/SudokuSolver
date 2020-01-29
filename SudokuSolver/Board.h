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
			
			Cell<_Ty, _N> cell(cellRow, cellCol, raw[i]);

			cells[i]                         = Cell<_Ty, _N>(cell);
			rows[cellRow][cellCol]           = cell; // Shallow copying
			cols[cellCol][cellRow]           = cell; // Shallow copying
			boxes[cells[i].getBoxIndex()][i] = cell; // Shallow copying
		}

	};
	~Board() {
		// RAII hopefully doesn't fail us here
	};
	
	// ===============================================================================
	//								Member Functions
	// ===============================================================================


};
