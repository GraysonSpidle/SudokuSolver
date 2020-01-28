from board import Board
from utils import *

if __name__ == "__main__":
    board = Board(rows=(
        (0,0,0,  0,0,0,  0,0,6),
        (1,3,0,  0,8,0,  0,2,0),
        (0,0,0,  0,0,5,  0,0,3),

        (5,0,0,  7,0,0,  0,1,0),
        (0,7,0,  2,0,1,  3,0,5),
        (0,0,0,  0,0,0,  9,0,2),

        (2,0,0,  3,0,6,  0,4,1),
        (0,0,0,  0,0,0,  0,0,0),
        (9,1,0,  0,7,0,  0,0,0)

    ))

    board.validate()
    marks_algorithm(board)
 
    
    orphan_marks_algorithm(board)

    print_board(board)

    print(board.row(8).raw_marks)

    raise Exception("")
