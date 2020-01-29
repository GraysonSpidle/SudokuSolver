#include "pch.h"
#include "Algorithms.h"

ALGORITHM_TEMPLATE
bool SudokuAlgorithms::removeMarkings(Unit<_Ty, _N> & unit) {
	bool mutated = false;
	for (_Ty i = 0; i < _N; ++i) {
		Cell<_Ty, _N> cell = unit.cell(i);
		if (cell.isEmpty())
			continue;

		for (_Ty k = 0; k < _N; ++k) {
			Cell<_Ty, _N> cell2 = unit.cell(i);
			if (!cell2.isEmpty() || cell == cell2)
				continue;

			mutated |= cell2.unmark(cell.getValue());
		}
	}
	return mutated;
}


