# Setting things up so we can import modules from the parent directory
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from board2 import Board

def marks_algorithm(board:Board):
    made_changes = False

    for rindex in range(9):
        row =  board.row(rindex)
        for cindex in range(9):
            cell = board.cell(rindex, cindex)
            col = board.col(cindex)
            box = board.box(rindex, cindex)

            if not cell.isBlank: 
                continue # Skip cells that aren't blank
            
            for candidate in range(1,10):
                if not any((candidate in row, candidate in col, candidate in box)):
                    made_changes = cell.mark(candidate) or made_changes
                else:
                    made_changes = cell.unmark(candidate) or made_changes
    if made_changes:
        marks_algorithm(board) # Loop recursively until no more changes are made

if __name__ == "__main__":
    ''' Tests this algorithm '''

    rows = (
        (0,6,0,  0,4,0,  0,5,0),
        (0,5,0,  0,0,0,  0,8,9),
        (0,0,3,  5,0,0,  4,0,0),

        (0,0,0,  0,0,1,  3,0,0),
        (9,0,0,  8,0,7,  0,0,1),
        (0,0,6,  2,0,0,  0,0,0),

        (0,0,9,  0,0,2,  7,0,0),
        (8,4,0,  0,0,0,  0,6,0),
        (0,7,0,  0,1,0,  0,3,0))

    board = Board(rows=rows)
    answer = Board(rows=rows)

    marks_algorithm(board)

    answer.cell(0,0).mark(1,2,7)
    answer.cell(0,2).mark(1,2,7,8)
    answer.cell(0,3).mark(1,3,7,9)
    answer.cell(0,5).mark(3,8,9)
    answer.cell(0,6).mark(1,2)
    answer.cell(0,8).mark(2,3,7)

    answer.cell(1,0).mark(1,2,4,7)
    answer.cell(1,2).mark(1,2,4,7)
    answer.cell(1,3).mark(1,3,6,7)
    answer.cell(1,4).mark(2,3,6,7)
    answer.cell(1,5).mark(3,6)
    answer.cell(1,6).mark(1,2,6)

    answer.cell(2,0).mark(1,2,7)
    answer.cell(2,1).mark(1,2,8,9)
    answer.cell(2,4).mark(2,6,7,8,9)
    answer.cell(2,5).mark(6,8,9)
    answer.cell(2,7).mark(1,2,7)
    answer.cell(2,8).mark(2,6,7)

    answer.cell(3,0).mark(2,4,5,7)
    answer.cell(3,1).mark(2,8)
    answer.cell(3,2).mark(2,4,5,7,8)
    answer.cell(3,3).mark(4,6,9)
    answer.cell(3,4).mark(5,6,9)
    answer.cell(3,7).mark(2,4,7,9)
    answer.cell(3,8).mark(2,4,5,6,7,8)

    answer.cell(4,1).mark(2,3)
    answer.cell(4,2).mark(2,4,5)
    answer.cell(4,4).mark(3,5,6)
    answer.cell(4,6).mark(2,5,6)
    answer.cell(4,7).mark(2,4)

    answer.cell(5,0).mark(1,3,4,5,7)
    answer.cell(5,1).mark(1,3,8)
    answer.cell(5,4).mark(3,5,9)
    answer.cell(5,5).mark(3,4,5,9)
    answer.cell(5,6).mark(5,8,9)
    answer.cell(5,7).mark(4,7,9)
    answer.cell(5,8).mark(4,5,7,8)

    answer.cell(6,0).mark(1,3,5,6)
    answer.cell(6,1).mark(1,3)
    answer.cell(6,3).mark(3,4,6)
    answer.cell(6,4).mark(3,5,6,8)
    answer.cell(6,7).mark(1,4)
    answer.cell(6,8).mark(4,5,8)

    answer.cell(7,2).mark(1,2,5)
    answer.cell(7,3).mark(3,7,9)
    answer.cell(7,4).mark(3,5,7,9)
    answer.cell(7,5).mark(3,5,9)
    answer.cell(7,6).mark(1,2,5,9)
    answer.cell(7,8).mark(2,5)

    answer.cell(8,0).mark(2,5,6)
    answer.cell(8,2).mark(2,5)
    answer.cell(8,3).mark(4,6,9)
    answer.cell(8,5).mark(4,5,6,8,9)
    answer.cell(8,6).mark(2,5,8,9)
    answer.cell(8,8).mark(2,4,5,8)

    board.validate()
    answer.validate()

    for r in range(9):
        for c in range(9):
            attempt = board.cell(r,c)
            actual = answer.cell(r,c)

            if attempt.marks != actual.marks:
                raise Exception("Marks in cell at row {} and col {} don't match the answer.".format(r,c))

    print("Marks test passed successfully")
