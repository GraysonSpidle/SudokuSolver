from cell import Cell
from sequence import Sequence
from board import Board

def marks_algorithm(board:Board):
    for rindex in range(9):
        row =  board.row(rindex)
        for cindex in range(9):
            cell = board.cell(rindex, cindex)
            col = board.col(cindex)
            box = board.box(rindex, cindex)

            if not cell.isBlank:
                continue

            for i in range(1,10):
                if not (i in row or i in col or i in box):
                    cell.mark(i)
                else:
                    cell.unmark(i)

def orphan_marks_algorithm(arg):
    if isinstance(arg, Cell):
        if len(arg.marks) == 1:
            arg.value = arg.marks[0]
            return True
        return False
    elif isinstance(arg, Sequence):
        made_a_change = False
        for m in range(1,10):
            if 1 in arg.count_marks(m):
                for cell in arg:
                    if m in cell.marks:
                        break
                cell.value = m
                made_a_change = True
                break
        return made_a_change
    elif isinstance(arg, Board):
        prev_made_a_change = False
        made_a_change = True
        while made_a_change or prev_made_a_change:
            prev_made_a_change = made_a_change
            made_a_change = any(orphan_marks_algorithm(cell) for cell in arg.cells()) or any(orphan_marks_algorithm(row) for row in arg.rows) or any(orphan_marks_algorithm(col) for col in arg.cols()) or any(orphan_marks_algorithm(box) for box in arg.boxes())
            marks_algorithm(arg)
        return made_a_change
    else:
        print("Err")

# TODO Implement moar algorithms
def xwing_algorithm(board:Board):
    # First, we need to identify xwings



    pass
    
def print_board(board:Board):
    for i in range(9):
        if i % 3 == 0 and i != 0:
            print("")
        for k in range(9):
            if k % 3 == 0 and k != 0:
                print("  ", end="")
            print(board.row(i)[k].value, end=" ")
        print()
        

