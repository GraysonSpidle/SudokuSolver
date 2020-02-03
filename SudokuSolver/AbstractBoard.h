#pragma once
#include <functional>
#include "AbstractCell.h"
#include "AbstractUnit.h"
#include "Utils.h"

SUDOKU_TEMPLATE
class AbstractBoard {
public:
	static constexpr const _Ty ROOT_N = SudokuUtils::isqrt(_N);

	virtual AbstractCell<_Ty, _N> & cell(_Ty i)        = 0; 
	virtual AbstractCell<_Ty, _N> & cell(_Ty r, _Ty c) = 0;
	virtual AbstractUnit<_Ty, _N> & row(_Ty i)         = 0;
	virtual AbstractUnit<_Ty, _N> & col(_Ty i)         = 0;
	virtual AbstractUnit<_Ty, _N> & box(_Ty i)         = 0;
};