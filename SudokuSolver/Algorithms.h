#pragma once
#include "Unit.h"

#define REQUIRE_UNSIGNED_NUMBER typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value>
#define ALGORITHM_TEMPLATE template <class _Ty, _Ty _N, REQUIRE_UNSIGNED_NUMBER>

namespace SudokuAlgorithms {
	// https://www.sudokuwiki.org/Strategy_Families

	ALGORITHM_TEMPLATE
	bool removeMarkings(Unit<_Ty, _N> & unit);
	/*bool removeMarkings(AbstractBoard & board);
	bool nakedSingle(AbstractSequence & seq);
	bool hiddenSingle(AbstractSequence & seq);
	bool hiddenSingle(AbstractBoard & board);
	bool hiddenSequence(AbstractSequence & seq);
	bool hiddenSequence(AbstractBoard & board);
	bool intersectionRemoval(AbstractBoard & board);
	bool xwing(AbstractBoard & board);
	bool ywing(AbstractBoard & board);*/
};

