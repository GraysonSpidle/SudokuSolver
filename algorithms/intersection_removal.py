# Setting things up so we can import modules from the parent directory
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from board2 import Board
from cell2 import Cell
from marks import marks_algorithm

def find_pair_or_triple_impl(board:Board, mark:int, rows_or_cols, row_or_col_key:callable) -> bool:
    made_changes = False
    filter_func = lambda _cell: mark in _cell # filters out all the cells that don't have the mark
    for sequence in rows_or_cols:
        filtered = filter(filter_func, sequence)
        for cell in filtered:
            filtered_box = [filter(filter_func, board.box_alternate(cell.box))]
            if len(filtered_box) < 2:
                continue
            ''' Checking the cell's box if there are other cells with the same mark that are in a different row/col and 
            we're also checking if there are 2 or 3 cells in the same row and box with the same mark
            '''
            bools = [row_or_col_key(_cell) == row_or_col_key(cell) for _cell in filtered_box]
            if not all(bools):
                continue
            ''' If we get here, then we've found a pair or a triple (doesn't really matter in the end)
            Now we're going to remove the mark from all the cells that don't share the same box but do share the same row/col
            '''
            # print("({},{}) {}".format(cell.row, cell.col, mark))
            for cell2 in filter(lambda _cell: _cell.box != cell.box, filtered):
                made_changes = cell2.unmark(mark) or made_changes

    return made_changes

def find_pair_or_triple(board:Board, mark:int) -> bool:
    row_key = lambda cell: cell.row
    col_key = lambda cell: cell.col
    return find_pair_or_triple_impl(board, mark, board.rows(), row_key) or find_pair_or_triple_impl(board, mark, board.cols(), col_key)

def intersection_removal_algorithm(board:Board) -> bool:
    made_changes = False
    for mark in range(1,10):
        made_changes = find_pair_or_triple(board, mark) or made_changes
    return made_changes

if __name__ == "__main__":
    ''' Tests the algorithm '''

    rows = (
        (0,3,2,  0,0,6,  1,0,0),
        (4,1,0,  0,0,0,  0,0,0),
        (0,0,0,  9,0,1,  0,0,0),

        (5,0,0,  0,9,1,  0,0,4),
        (0,6,0,  0,0,0,  0,7,1),
        (3,0,0,  0,2,0,  0,0,5),

        (0,0,0,  5,0,8,  0,0,0),
        (0,0,0,  0,0,0,  5,1,9),
        (0,5,7,  0,0,9,  8,6,0),
    )

    board = Board(rows=rows)
    answer = Board(rows=rows)

    marks_algorithm(board)
    marks_algorithm(answer)
    
    made_changes = intersection_removal_algorithm(board)
    while made_changes:
        made_changes = intersection_removal_algorithm(board)
    
    answer.cell(1,5).unmark(7)
    answer.cell(1,6).unmark(2,6)
    answer.cell(1,7).unmark(2,8)
    answer.cell(1,8).unmark(2)

    answer.cell(2,1).unmark(7)
    answer.cell(2,6).unmark(6)
    answer.cell(2,7).unmark(8)

    answer.cell(4,0).unmark(8)
    answer.cell(4,2).unmark(8)

    answer.cell(6,1).unmark(4)
    answer.cell(6,2).unmark(1,4)
    answer.cell(6,4).unmark(4,7)

    answer.cell(7,3).unmark(4)
    answer.cell(7,4).unmark(4)
    answer.cell(7,5).unmark(4,7)

    board.validate()
    answer.validate()

    for r in range(9):
        for c in range(9):
            attempt = board.cell(r,c)
            actual = answer.cell(r,c)

            if attempt.marks != actual.marks:
                raise Exception("Marks in cell at row {} and col {} do not match answer.".format(r,c))

    print("Intersection removal test passed successfully.")


