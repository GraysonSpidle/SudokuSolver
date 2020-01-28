from cell2 import Cell

class Board:
    def __init__(self, rows=None):
        self._cells = []
        for r in range(9):
            for c in range(9):
                if rows is None:
                    self._cells.append(Cell(r,c))
                else:
                    self._cells.append(Cell(r,c,rows[r][c]))
    
    def row(self, index:int):
        return tuple(filter(lambda cell: cell.row == index, self._cells))

    def rows(self):
        return tuple(self.row(i) for i in range(9))
    
    def col(self, index:int):
        return tuple(filter(lambda cell: cell.col == index, self._cells))

    def cols(self):
        return tuple(self.col(i) for i in range(9))

    def cell(self, rindex:int, cindex:int) -> Cell:
        return next(filter(lambda cell: cell.row == rindex and cell.col == cindex, self._cells))

    def cells(self):
        return self._cells

    def box(self, rindex:int, cindex:int):
        output = []
        rbegin, cbegin = (rindex // 3) * 3, (cindex // 3) * 3
        for r in range(rbegin, rbegin + 3):
            for c in range(cbegin, cbegin + 3):
                output.append(self.cell(r,c))
        return output

    def box_alternate(self, bindex:int):
        rindex = (bindex // 3) * 3
        cindex = (bindex % 3) * 3
        return self.box(rindex, cindex)

    def boxes(self) -> tuple:
        return tuple(self.box_alternate(i) for i in range(9))

    def _validate_cells(self, cells):
        for i in range(1,10):
            try:
                first = cells.index(i)
                second = cells.index(i, first + 1) # If this doesn't raise ValueError, then more than 2 cells have the same value
                raise Exception("Invalid Sequence: cells at ({},{}) and ({},{}) share the same value.".format(cells[first].row, cells[first].col, cells[second].row, cells[second].col))
            except ValueError:
                continue

    def validate(self) -> None:
        self._validate_cells(self.rows())
        self._validate_cells(self.cols())
        self._validate_cells(self.boxes()) 
            
