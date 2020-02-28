#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here

#endif //PCH_H

#ifndef SUDOKU_TEMPLATE
/* Basically this macro is the template for all the sudoku stuff.
It asks for a class _Ty that must be an unsigned number (so a non-floating point data type), and an
instance of that number _N which represents the length of the board in cells (so a traditional 9x9 board would have _N = 9)
*/
#define SUDOKU_TEMPLATE\
	template <\
		class _Ty,\
		const _Ty _N,\
		typename = std::enable_if<\
			std::is_arithmetic_v<_Ty> &&\
			std::is_unsigned_v<_Ty>\
		>\
	>
#endif


