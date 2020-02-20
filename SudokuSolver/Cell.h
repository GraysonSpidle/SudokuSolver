#pragma once
#include <array>
#include <algorithm>
#include <cassert>
#include <vector>
#include "AbstractCell.h"
#include "Utils.h"

SUDOKU_TEMPLATE
class Cell : public AbstractCell<_Ty, _N> {
	_Ty						 rowNum;
	_Ty						 colNum;
	_Ty						  value;
	bool				  _isStatic;
	std::array<bool, _N>   markings;
public:
	// ===============================================================================
	//							 Constructors / Destructors
	// ===============================================================================

	Cell(_Ty && row, _Ty && col, _Ty && value, bool isStatic = false) :
		rowNum(row),
		colNum(col),
		value(value),
		markings(),
		_isStatic(isStatic) {

		resetMarks();
	};

	explicit Cell(_Ty && value)
		: Cell(0, 0, value, value != 0) {};

	Cell() 
		: Cell(0, 0, 0, false) {};
	
	~Cell() {};

	// ===============================================================================
	//								Member Functions
	// ===============================================================================

	// gets non-empty marks. will return empty values if there are no more non-empty markings.
	_Ty operator[](_Ty i) override {
		if (!isEmpty()) // populated cells don't have markings
			return 0;
		_Ty n = 0;
		for (_Ty k = 0; k < _N; ++k) {
			if (markings[k]) {
				if (n == i)
					return k + 1;
				else
					n++;
			}
		}
		return 0;
	}

	bool operator==(const AbstractCell<_Ty, _N> & other) const override {
		return value == other.getValue() && rowNum == other.getRowIndex() && colNum == other.getColIndex() && getBoxIndex() == other.getBoxIndex();
	}

	bool operator==(const Cell<_Ty, _N> & other) const {
		return rowNum == other.rowNum && colNum == other.colNum && getBoxIndex() == other.getBoxIndex() && markings == other.markings;
	}

	bool isEmpty() const override { return value == 0; }
	bool isStatic() const override { return _isStatic; }
	void setStatic(bool _static) {
		_isStatic = _static;
	}

	_Ty getValue() const override { return this->value; }

	bool setValue(_Ty newValue) override {
		value = newValue;
		return true;
	}

	_Ty getRowIndex() const override { return rowNum; }
	void setRowIndex(_Ty index) override {
		assert(index < _N);
		rowNum = index; 
	}
	_Ty getColIndex() const override { return colNum; }
	void setColIndex(_Ty index) override {
		assert(index < _N);
		colNum = index;
	}
	_Ty getBoxIndex() const override { 
		return this->ROOT_N * (rowNum / this->ROOT_N) + (colNum / this->ROOT_N);
	}

	bool containsMark(const _Ty & marking) const override {
		// Since all possible markings are in a range from 1 to _N, we can just use marking as an index.
		if (!isEmpty())
			return false;
		return markings[marking - 1];
	}

	_Ty countMarks() const {
		return std::count_if(markings.cbegin(), markings.cend(), [](const bool b) {
			return b;
		});
	}

	bool mark(const _Ty & marking) override {
		// Since all possible markings are in a range from 1 to _N, we can just use marking as an index.
		if (containsMark(marking) || !isEmpty())
			return false;
		markings[marking - 1] = true;
		return true;
	}

	bool unmark(const _Ty & marking) override {
		// Since all possible markings are in a range from 1 to _N, we can just use marking as an index.
		if (!containsMark(marking) || !isEmpty())
			return false;
		markings[marking - 1] = false;
		return true;
	}

	bool resetMarks() override {
		for (_Ty i = 0; i < _N; ++i) {
			markings[i] = true;
		}
		return true;
	}

	std::string toString() const {
		return std::to_string(value);
	}

};

