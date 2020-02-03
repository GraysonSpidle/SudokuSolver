#pragma once
#include <type_traits>

#define SUDOKU_UTILS_TEMPLATE template <class _Ty, typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value>>

namespace SudokuUtils {
	SUDOKU_UTILS_TEMPLATE
	constexpr const _Ty isqrt_impl(_Ty sq, _Ty dlt, _Ty value) {
		// I got this from some where, I just added the template part.
		return sq <= value ?
			isqrt_impl<_Ty>(sq + dlt, dlt + 2, value) : (dlt >> 1) - 1;
	}

	SUDOKU_UTILS_TEMPLATE
	constexpr const _Ty isqrt(_Ty value) {
		// I got this from some where, I just added the template part.
		return isqrt_impl<_Ty>(1, 3, value);
	}

	SUDOKU_UTILS_TEMPLATE
	constexpr _Ty calculateBoxIndex(_Ty row, _Ty col, _Ty _N) {
		/* Box indices are laid out like this (for a traditional 9x9 sudoku board)
				|		|		|
			0	|	1	|	2	|
				|		|		|
		-------------------------
				|		|		|
			3	|	4	|	5	|
				|		|		|
		-------------------------
				|		|		|
			6	|	7	|	8	|
				|		|		|

		*/
		_Ty rootSize = isqrt(_N);
		_Ty num1 = row / rootSize; // This should be the same thing as floor division since doubles and floats cannot be unsigned
		_Ty num2 = col / rootSize; // This should be the same thing as floor division since doubles and floats cannot be unsigned
		return rootSize * num1 + num2;
	};

	SUDOKU_UTILS_TEMPLATE
	constexpr std::pair<_Ty, _Ty> calculateLocalBoxCoords(_Ty row, _Ty col, _Ty _N) {
		_Ty bindex = calculateBoxIndex(row, col, _N);
		_Ty rootSize = isqrt(_N);
		return { row - rootSize * floor(bindex / rootSize), col - rootSize * (bindex % rootSize) };
	};

}

