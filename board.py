from cell import Cell
from sequence import Sequence

class Board:
    def __init__(self, rows=None):
        if rows is None:
            self.rows = tuple(Sequence() for i in range(9))
        else:
            self.rows = tuple(Sequence(row) for row in rows)
    
    def row(self, index:int) -> Sequence:
        return self.rows[index]
    
    def col(self, index:int) -> Sequence:
        return Sequence((self.rows[i][index].value for i in range(len(self.rows))))

    def cols(self) -> tuple:
        return tuple(self.col(i) for i in range(9))

    def cell(self, rindex:int, cindex:int) -> Cell:
        return self.rows[rindex][cindex]
    
    def _get_cells_iter(self):
        for rindex in range(9):
            for cindex in range(9):
                yield self.cell(rindex, cindex)
    
    def cells(self) -> tuple:
        return tuple(self._get_cells_iter())

    def box(self, rindex:int, cindex:int) -> Sequence:
        output = []
        rbegin, cbegin = (rindex // 3) * 3, (cindex // 3) * 3
        for r in range(rbegin, rbegin + 3):
            for c in range(cbegin, cbegin + 3):
                output.append(self.cell(r,c).value)
        return Sequence(raw_values=output)

    def box_alternate(self, bindex:int) -> Sequence:
        rindex = (bindex // 3) * 3
        cindex = (bindex % 3) * 3
        return self.box(rindex, cindex)

    def boxes(self) -> tuple:
        return tuple(self.box_alternate(i) for i in range(9))

    def validate(self) -> None:
        for index in range(9):
            self.row(index).validate()
            self.col(index).validate()
            self.box_alternate(index).validate()

