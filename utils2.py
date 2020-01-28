from cell2 import Cell
from board2 import Board

def intersection(*args):
    output = []
    for e in (collection for collection in args):
        if e in output:
            continue
        elif all(e in collection for collection in args):
            output.append(e)
    return output

def index_cells_by_marks(*cells):
    ''' This function will put cells with other cells that contain the same mark. For example, output[1] would return a list
    of cells that are all marked with 1.
    '''
    output = [list() for i in range(10)] # b/c 0 indicates a blank spot and no cell should have a 0 mark
    for cell in filter(lambda cell: cell.isBlank, cells):
        for mark in cell.marks:
            output[mark].append(cell)
    return output





