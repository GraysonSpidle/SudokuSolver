# Setting things up so we can import modules from the parent directory
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from board2 import Board
from marks import marks_algorithm
from hidden_single import hidden_single_algorithm
from utils2 import index_cells_by_marks, intersection

def hidden_sequence_algorithm(board:Board) -> bool:
    made_changes = False
    if isinstance(board, Board):
        for sequence in board.rows() + board.cols(): # Excluding boxes b/c this algorithm only works for rows and cols
            made_changes = hidden_sequence_algorithm(sequence) or made_changes
    elif isinstance(board, tuple) or isinstance(board, list): # Now we're working with individual rows/cols
        blank_cells = tuple(filter(lambda cell: cell.isBlank and len(cell.marks) > 1, board)) # Collecting only the blank cells
        if len(blank_cells) < 2:
            return False # the algorithm only works for rows and cols w/ 2+ blank cells that have 2+ marks
        del blank_cells

        mapped = index_cells_by_marks(board)

        for mark in range(1, len(mapped)): # Iterate thru all the possible marks excluding 0
            grouping = mapped[mark]
            shared_marks = intersection(*(cell.marks for cell in grouping)) # finding the other marks these cells have in common

            ''' Now we're going to eliminate the marks in shared marks that are present in other cells outside of this particular grouping.
            For example, if 2 cells shared the marks 2,3, and 5 while a third cell had 5, then we're going to remove the 5 from the first two cells.
            This is logical because if the 5 was in any of the first two cells, there would be no other place for either the 2 or the 3 to go
            therefore the 5 cannot be in the first 2 cells.
            '''
            shared_marks = list(filter(lambda shared_mark: shared_mark == mark or len(mapped[shared_mark]) == len(grouping), shared_marks))

            ''' Now we're just checking if that elimination technique worked (It should have worked or else there's an error) '''
            if len(shared_marks) != len(grouping):
                raise Warning("Invalid marking setup detected. There are {} cells that are the only cells in the sequence marked with {}".format(len(grouping), shared_marks))
            elif (len(shared_marks) == len(grouping) and len(shared_marks) == 1) and hidden_single_algorithm(board):
                return True # This is here because the cells' marks would need to be refreshed.
            else:
                for cell in grouping:
                    ''' We're going to remove all marks from all the cells in the grouping that aren't included in shared_marks. '''
                    made_changes = cell.unmark(*filter(lambda i: not i in shared_marks, range(1, len(board) + 1))) or made_changes
                if made_changes: # This is here because if changes were made, then this algorithm might mess up. The cells' marks need to be refreshed.
                    return True
    return made_changes

if __name__ == "__main__":
    ''' Tests the algorithm '''

    # Go here for the example: http://www.sudokuwiki.org/Hidden_Candidates
    rows = (
        (0,0,0,  0,0,1,  0,3,0),
        (2,3,1,  0,9,0,  0,0,0),
        (0,6,5,  0,0,3,  1,0,0),

        (6,7,8,  9,2,4,  3,0,0),
        (1,0,3,  0,5,0,  0,0,6),
        (0,0,0,  1,3,6,  7,0,0),

        (0,0,9,  3,6,0,  5,7,0),
        (0,0,6,  0,1,9,  8,4,3),
        (3,0,0,  0,0,0,  0,0,0)
    )
    
    board = Board(rows=rows)
    answer = Board(rows=rows)

    while hidden_sequence_algorithm(board):
        marks_algorithm(board)

    
    


