#pragma once
#include <array>
#include <algorithm>
#include <set>
#include "Utils.h"

SUDOKU_TEMPLATE
class AbstractCell {
public:
	static constexpr const _Ty ROOT_N = SudokuUtils::isqrt(_N);

	virtual _Ty operator[](_Ty i) = 0;
	virtual bool operator==(const AbstractCell<_Ty, _N> & other) const = 0;
	bool operator!=(const AbstractCell<_Ty, _N> & other) const {
		return !this->operator==(other);
	};

	virtual bool isEmpty() const = 0;
	virtual bool isStatic() const = 0;
	virtual _Ty getValue() const = 0;
	virtual bool setValue(_Ty arg0) = 0;
	virtual _Ty getRowIndex() const = 0;
	virtual _Ty getColIndex() const = 0;
	virtual _Ty getBoxIndex() const = 0;
	virtual void setRowIndex(_Ty arg0) = 0;
	virtual void setColIndex(_Ty arg0) = 0;
	virtual bool containsMark(const _Ty & marking) const = 0;
	virtual bool mark(const _Ty & marking) = 0;
	virtual bool unmark(const _Ty & marking) = 0;
	virtual bool resetMarks() = 0;
};

