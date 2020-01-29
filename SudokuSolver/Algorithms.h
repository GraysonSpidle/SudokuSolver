#pragma once
#include "Cell.h"
#include "Unit.h"
#include "Board.h"

#define REQUIRE_UNSIGNED_NUMBER typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value>
#define ALGORITHM_TEMPLATE template <class _Ty, _Ty _N, REQUIRE_UNSIGNED_NUMBER>

namespace SudokuAlgorithms {
	// https://www.sudokuwiki.org/Strategy_Families

	ALGORITHM_TEMPLATE
	bool removeMarkings(Unit<_Ty, _N> & unit);
	ALGORITHM_TEMPLATE
	bool removeMarkings(Board<_Ty, _N> & board);
	ALGORITHM_TEMPLATE
	bool nakedSingle(Unit<_Ty, _N> & unit);
	ALGORITHM_TEMPLATE
	bool nakedSingle(Board<_Ty, _N> & board);
	ALGORITHM_TEMPLATE
	bool hiddenSingle(Unit<_Ty, _N> & unit);
	ALGORITHM_TEMPLATE
	bool hiddenSingle(Board<_Ty, _N> & board);
	ALGORITHM_TEMPLATE
	bool hiddenSequence(Unit<_Ty, _N> & unit);
	ALGORITHM_TEMPLATE
	bool hiddenSequence(Board<_Ty, _N> & board);
	ALGORITHM_TEMPLATE
	bool intersectionRemoval(Board<_Ty, _N> & board);
	ALGORITHM_TEMPLATE
	bool xwing(Board<_Ty, _N> & board);
	ALGORITHM_TEMPLATE
	bool ywing(Board<_Ty, _N> & board);
};

