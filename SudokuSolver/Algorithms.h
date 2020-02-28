#pragma once
#include <map>
#include <vector>
#include <set>
#include <memory>
#include <functional>

#include "AbstractUnit.h"
#include "AbstractBoard.h"
#include "Graph.h"
#include "Edge.h"

#ifndef SUDOKU_TEMPLATE_EXTRA
/* See SUDOKU_TEMPLATE for details. This macro is just for my algorithms which does exactly the same thing
as SUDOKU_TEMPLATE, except it defines some convenience typenames so I don't have to put 
std::shared_ptr<AbstractCell<_Ty, _N>> for every time I'm talking about a cell.
*/
#define SUDOKU_TEMPLATE_EXTRA\
	template <\
		class _Ty,\
		const _Ty _N,\
		typename = std::enable_if<\
			std::is_arithmetic_v<_Ty> &&\
			std::is_unsigned_v<_Ty>\
		>,\
		typename CellPtr = std::shared_ptr<AbstractCell<_Ty, _N>>,\
		typename MarkingMap = std::map<_Ty, std::set<CellPtr>>,\
		typename HSGraph = Graph< CellPtr, Edge<CellPtr, _Ty> >\
	>
#endif


namespace SudokuAlgorithms {
	// https://www.sudokuwiki.org/Strategy_Families

	SUDOKU_TEMPLATE_EXTRA
	bool removeMarkings(AbstractUnit<_Ty, _N> & unit);
	SUDOKU_TEMPLATE_EXTRA
	bool removeMarkings(AbstractBoard<_Ty, _N> & board);
	SUDOKU_TEMPLATE_EXTRA
	bool nakedSingle(AbstractUnit<_Ty, _N> & unit);
	SUDOKU_TEMPLATE_EXTRA
	bool nakedSingle(AbstractBoard<_Ty, _N> & board);
	SUDOKU_TEMPLATE_EXTRA
	bool hiddenSingle(AbstractUnit<_Ty, _N> & unit);
	SUDOKU_TEMPLATE_EXTRA
	bool hiddenSingle(AbstractBoard<_Ty, _N> & board);
	SUDOKU_TEMPLATE_EXTRA
	bool hiddenSequence(AbstractUnit<_Ty, _N> & unit);
	SUDOKU_TEMPLATE_EXTRA
	bool hiddenSequence(AbstractBoard<_Ty, _N> & board);
	SUDOKU_TEMPLATE_EXTRA
	bool intersectionRemoval(AbstractBoard<_Ty, _N> & board);
	SUDOKU_TEMPLATE_EXTRA
	bool xwing(AbstractBoard<_Ty, _N> & board);
	SUDOKU_TEMPLATE_EXTRA
	bool ywing(AbstractBoard<_Ty, _N> & board);

};

