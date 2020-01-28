# Setting things up so we can import modules from the parent directory
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from board2 import Board
from cell2 import Cell
from marks import marks_algorithm
from utils2 import index_cells_by_marks, intersection

def find_xwing_impl(cell:Cell, rows_or_cols:list, rows_or_cols2:list, key:callable, key2:callable, mark:int) -> bool:
    ''' This finds the classic x-wing '''
    for cell2 in filter(lambda cell2: len(intersection(*cell.marks, *cell2.marks)) == len(cell.marks) and cell.box != cell2.box, rows_or_cols[key(cell)]):
        for sequence in rows_or_cols:
            possible_cells = index_cells_by_marks(*sequence)[mark]
            if key(sequence[0]) == key(cell) or any(c.box == cell.box or c.box == cell2.box for c in sequence) or len(possible_cells) < 2:
                continue
            elif any(key2(cell) == key2(pc) for pc in possible_cells) and any(key2(cell2) == key2(pc) for pc in possible_cells):
                ''' Now that we've identified our x-wing, we're going to remove `mark` from the other cells that share columns/rows with these cells '''
                if any(_cell.unmark(mark) for _cell in rows_or_cols2[key2(cell.col)] + rows_or_cols2[key2(cell2)]):
                    return True
                # If we make it here, then the x-wing we just identified, didn't yield any changes to the board :(
    return False

def find_xwing(cell:Cell, rows:list, cols:list, mark:int) -> bool:
    row_key = lambda cell: cell.row
    col_key = lambda cell: cell.col
    return find_xwing_impl(cell, rows, cols, row_key, col_key, mark) or find_xwing_impl(cell, cols, rows, col_key, row_key, mark)

def xwing_algorithm(board:Board) -> bool:
    # If a digit is a candidate for exactly two cells (in two different rows and boxes) and the columns of those two cells are exactly the same.
    # then this digit cannot be a candidate for any other rows in the same two columns.
    for mark in range(1, 10):
        ''' Filtering out all the cells that don't have `mark` '''
        cells = list(filter(lambda cell: mark in cell.marks, board.cells()))

        if len(cells) == 1:
            continue

        cols = [list(filter(lambda cell: cell.col == i, cells)) for i in range(9)]
        rows = [list(filter(lambda cell: cell.row == i, cells)) for i in range(9)]

        for cell in filter(lambda cell: len(cell.marks) == 2, cells):
            made_changes = find_xwing(cell, rows, cols, mark)
            if made_changes:
                return True
    return False


if __name__ == "__main__":

    # Go here for example: http://www.sudokuwiki.org/X_Wing_Strategy

    rows = (

    )

