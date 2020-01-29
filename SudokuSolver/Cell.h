#pragma once
#include <array>
#include <algorithm>
#include <cassert>
#include <vector>
#include "Utils.h"

template <
	class _Ty, // the data type to use for the values and the markings
	_Ty _N, // how many total markings there could be
	typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value> // requiring that _Ty is an unsigned number
>
class Cell {
	_Ty				         row;
	_Ty                      col;
	_Ty                      box;
	_Ty                    value;
	bool               _isStatic;
	std::array<_Ty, _N> markings;
	
	constexpr const _Ty getBoxIndex(_Ty row, _Ty col) {
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
		_Ty rootSize = SudokuUtils::isqrt(_N);
		_Ty num1 = row / rootSize; // This should be the same thing as floor division since doubles and floats cannot be unsigned
		_Ty num2 = col / rootSize; // This should be the same thing as floor division since doubles and floats cannot be unsigned
		return rootSize * num1 + num2;
	};
public:
	static const _Ty EMPTY_VALUE = 0;

	// ===============================================================================
	//							 Constructors / Destructors
	// ===============================================================================

	Cell(_Ty row, _Ty col, _Ty value, bool isStatic, std::array<_Ty, _N> && markings) 
		: row(row), col(col), box(getBoxIndex(row, col)) {
		this->value = value;
		this->_isStatic = isStatic;
		this->markings = markings;
		
		assert(!isStatic && this->isEmpty()); // We must ensure that any cell constructed cannot be static and empty.
	};

	Cell(_Ty row, _Ty col, _Ty value, bool isStatic)
		: value(value), row(row), col(col), box(getBoxIndex(row, col)), _isStatic(isStatic) {
		
		assert((!_isStatic && isEmpty()) || (_isStatic && !isEmpty())); // We must ensure that any cell constructed cannot be static and empty.
		
		if (isEmpty()) {
			markings = std::array<_Ty, _N>();
			for (_Ty i = 0; i < _N; ++i) {
				markings[i] = i + 1;
			}
		}
	};

	Cell(const _Ty row, const _Ty col, _Ty value)
		: Cell<_Ty, _N>(row, col, value, value != Cell<_Ty, _N>::EMPTY_VALUE) {};

	explicit Cell(_Ty value)
		: Cell<_Ty, _N>(0, 0, value) {};

	Cell() 
		: Cell<_Ty, _N>(Cell<_Ty, _N>::EMPTY_VALUE) {};
	
	~Cell() {
		// Hopefully RAII will save us.
	};

	// ===============================================================================
	//								Member Functions
	// ===============================================================================

	// gets non-empty marks will return empty values if there are no more non-empty markings
	_Ty & operator[](_Ty i) {
		if (!isEmpty())
			return EMPTY_VALUE;
		_Ty n = 0;
		for (_Ty k = 0; k < _N; ++k) {
			if (markings[k] != EMPTY_VALUE) {
				if (n == i)
					return markings[k];
				else
					n++;
			}
		}
		return EMPTY_VALUE;
	}

	bool isEmpty() const { return value == Cell<_Ty, _N>::EMPTY_VALUE; }
	bool isStatic() const { return _isStatic; }
	void setStatic(bool _static) {
		_isStatic = _static;
	}
	
	_Ty & getValue() const { return value; }

	bool setValue(_Ty & newValue) {
		value = newValue;
		return true;
	}

	const _Ty & getRowIndex() const { return row; }
	void setRowIndex(_Ty & index) { 
		assert(index < _N);
		row = index; 
		box = getBoxIndex(row, col);
	}
	const _Ty & getColIndex() const { return col; }
	void setColIndex(_Ty & index) {
		assert(index < _N);
		col = index;
		box = getBoxIndex(row,col);
	}
	const _Ty & getBoxIndex() const { return box; }

	bool containsMark(const _Ty & marking) const {
		if (!isEmpty())
			return false;
		return std::find(markings.cbegin(), markings.cend(), marking) != markings.cend();
	}

	_Ty countMarks() const {
		return std::count_if(markings.cbegin(), markings.cend(), [&EMPTY_VALUE](const _Ty & marking) {
			return marking != EMPTY_VALUE;
		});
	}

	bool mark(_Ty & marking) {
		// Ensuring that the cell doesn't contain the marking first (so we don't have duplicates)
		if (containsMark(marking))
			return false;
		// Finding the first empty spot in the array to put our marking in
		for (_Ty i = 0; i < _N; ++i) {
			if (markings[i] == Cell<_Ty, _N>::EMPTY_VALUE) {
				markings[i] = marking;
				return true;
			}
		}
		return false;
	}

	bool unmark(_Ty & marking) {
		for (_Ty i = 0; i < _N; ++i) {
			if (markings[i] == marking) {
				markings[i] = Cell<_Ty, _N>::EMPTY_VALUE;
				return true;
			}
		}
		return false;
	}

	bool resetMarks() {
		for (_Ty i = 0; i < _N; ++i) {
			markings[i] = i + 1;
		}
		return true;
	}

};

