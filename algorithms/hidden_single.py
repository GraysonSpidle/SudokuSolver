# Setting things up so we can import modules from the parent directory
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from board2 import Board
from marks import marks_algorithm

def hidden_single_algorithm(board:Board) -> bool: # This should also do something similar to the naked single algorithm
    if isinstance(board, Board):
        for sequence in board.rows() + board.cols() + board.boxes():
            if hidden_single_algorithm(sequence):
                return True
    elif isinstance(board, tuple) or isinstance(board, list): # Now we're dealing with rows, cols, or boxes
        for i in range(1,10):
            cells = tuple(filter(lambda cell: i in cell, board)) # filters every cell with the mark in the row/col/box
            if len(cells) == 1:
                cells[0].value = i
                return True   
    return False # Didn't make changes

if __name__ == "__main__":
    ''' Tests this algorithm '''

    rows = (
        (0,0,0,  8,0,2,  0,0,1),
        (0,9,0,  6,0,1,  0,4,0),
        (5,0,0,  0,0,0,  0,0,6),

        (0,2,3,  0,0,0,  8,6,0),
        (8,7,0,  0,6,0,  0,9,2),
        (6,5,9,  0,0,0,  1,7,0),

        (3,0,0,  0,0,0,  4,0,9),
        (0,4,0,  7,0,3,  6,1,0),
        (0,0,0,  4,0,6,  0,0,0))

    board = Board(rows=rows)
    answer = Board(rows=rows)

    marks_algorithm(board)
    marks_algorithm(answer)

    while hidden_single_algorithm(board):
        marks_algorithm(board) # Refreshes the marks

    answer.cell(6,2).value = 7
    answer.cell(0,2).value = 6
    answer.cell(6,1).value = 6

    board.validate()
    answer.validate()

    for r in range(9):
        for c in range(9):
            attempt = board.cell(r,c)
            actual = answer.cell(r,c)

            if attempt.value != actual.value:
                raise Exception("Value in cell at row {} and col {} don't match the answer.".format(r,c))

    print("Hidden single test passed successfully")

