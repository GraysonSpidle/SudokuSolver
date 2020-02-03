#pragma once
#include <array>
#include <algorithm>
#include "AbstractCell.h"
#include "Utils.h"

SUDOKU_TEMPLATE
class AbstractUnit {
public:
	static constexpr const _Ty ROOT_N = SudokuUtils::isqrt(_N);

	virtual AbstractCell<_Ty, _N> & operator[](_Ty i) = 0;
	virtual AbstractCell<_Ty, _N> & cell(_Ty i) = 0;
	virtual AbstractCell<_Ty, _N> & cell(_Ty boxCellRow, _Ty boxCellCol) = 0;
	virtual _Ty countCellsMarkedWith(_Ty marking) const = 0;
};
