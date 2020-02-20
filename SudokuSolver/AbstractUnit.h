#pragma once
#include <array>
#include <functional>
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

	bool anyOf(std::function<bool(AbstractCell<_Ty, _N>&)> pred) {
		for (_Ty i = 0; i < _N; ++i) {
			if (pred(cell(i)))
				return true;
		}
		return false;
	}
};
