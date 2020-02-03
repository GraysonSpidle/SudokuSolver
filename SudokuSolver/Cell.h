#pragma once
#include <array>
#include <algorithm>
#include <memory>
#include <cassert>
#include <vector>
#include "AbstractCell.h"
#include "Utils.h"

SUDOKU_TEMPLATE
class Cell : public AbstractCell<_Ty, _N> {
	using ptr = std::shared_ptr<_Ty>;

	ptr      rowNum;
	ptr      colNum;
	ptr       value;
	bool   _isStatic;
public:
	std::shared_ptr<std::array<_Ty, _N>> markings;

	// ===============================================================================
	//							 Constructors / Destructors
	// ===============================================================================

	Cell(_Ty && row, _Ty && col, _Ty && value, bool isStatic = false)
		: rowNum(new _Ty(row)), colNum(new _Ty(col)), value(new _Ty(value)), _isStatic(isStatic), markings(new std::array<_Ty, _N>) {
		for (_Ty i = 0; i < _N; ++i) {
			(*markings)[i] = i + 1;
		}
	};

	explicit Cell(_Ty && value)
		: Cell(0, 0, value, value != 0) {};

	Cell() 
		: Cell(0, 0, 0, false) {};
	
	~Cell() {
		// RAII pls halp w/ memory management
	};

	// ===============================================================================
	//								Member Functions
	// ===============================================================================

	// gets non-empty marks will return empty values if there are no more non-empty markings
	_Ty operator[](_Ty i) override {
		if (!isEmpty())
			return 0;
		_Ty n = 0;
		for (_Ty k = 0; k < _N; ++k) {
			if ((*markings)[k] != 0) {
				if (n == i)
					return (*markings)[k];
				else
					n++;
			}
		}
		return 0;
	}
	bool operator==(const AbstractCell<_Ty, _N> & other) const override {
		return *value == other.getValue() && *rowNum == other.getRowIndex() && *colNum == other.getColIndex() && getBoxIndex() == other.getBoxIndex();
	}

	bool operator==(const Cell<_Ty, _N> & other) const {
		return rowNum == other.rowNum && colNum == other.colNum && getBoxIndex() == other.getBoxIndex() && markings == other.markings;
	}

	bool isEmpty() const override { return (*value) == 0; }
	bool isStatic() const override { return _isStatic; }
	void setStatic(bool _static) {
		_isStatic = _static;
	}

	_Ty getValue() const override { return *this->value; }

	bool setValue(_Ty newValue) override {
		*value = newValue;
		return true;
	}

	_Ty getRowIndex() const override { return *rowNum; }
	void setRowIndex(_Ty index) override {
		assert(index < _N);
		*rowNum = index; 
	}
	_Ty getColIndex() const override { return *colNum; }
	void setColIndex(_Ty index) override {
		assert(index < _N);
		*colNum = index;
	}
	_Ty getBoxIndex() const override { 
		return this->ROOT_N * ((*rowNum) / this->ROOT_N) + ((*colNum) / this->ROOT_N);
	}

	bool containsMark(const _Ty & marking) const override {
		if (!isEmpty())
			return false;
		return std::find(markings->cbegin(), markings->cend(), marking) != markings->cend();
	}

	_Ty countMarks() const {
		return std::count_if(markings->cbegin(), markings->cend(), [](const _Ty & marking) {
			return marking != 0;
		});
	}

	bool mark(const _Ty & marking) override {
		// Ensuring that the cell doesn't contain the marking first (so we don't have duplicates)
		if (containsMark(marking))
			return false;
		// Finding the first empty spot in the array to put our marking in
		for (_Ty i = 0; i < _N; ++i) {
			if ((*markings)[i] == 0) {
				(*markings)[i] = marking;
				return true;
			}
		}
		return false;
	}

	bool unmark(const _Ty & marking) override {
		for (_Ty i = 0; i < _N; ++i) {
			if ((*markings)[i] == marking) {
				(*markings)[i] = 0;
				return true;
			}
		}
		return false;
	}

	bool resetMarks() override {
		for (_Ty i = 0; i < _N; ++i) {
			(*markings)[i] = i + 1;
		}
		return true;
	}

	std::string toString() const {
		return std::to_string(*value);
	}

};

