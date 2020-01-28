from cell2 import Cell

class Sequence:
    def __init__(self, index:int, raw_values=[0,0,0, 0,0,0, 0,0,0]):
        self.index = index
        self.cells = [Cell(raw_value) for raw_value in raw_values]

    def __setitem__(self, key:int, value:Cell):
        self.cells[key] = value

    def __getitem__(self, y:int) -> Cell:
        return self.cells[y]
    
    def __iter__(self):
        return iter(self.cells)

    def __len__(self) -> int:
        return len(self.cells)

    def _get_raw_values_iter(self, reverse:bool=False):
        return map(lambda cell: cell.value,
        self.cells if not reverse else reversed(self.cells))

    @property
    def raw_values(self) -> tuple:
        return tuple(self._get_raw_values_iter(False))

    @property
    def raw_values_reverse(self) -> tuple:
        return tuple(self._get_raw_values_iter(True))

    def _get_raw_marks_iter(self, reverse:bool=False):
        return map(lambda cell: cell.marks,
        self.cells if not reverse else reversed(self.cells))

    @property
    def raw_marks(self) -> tuple:
        return tuple(self._get_raw_marks_iter(False))
    
    @property
    def raw_marks_reverse(self) -> tuple:
        return tuple(self._get_raw_marks_iter(True))

    def __contains__(self, element) -> bool:
        if isinstance(element, Cell):
            return element in self.cells
        elif isinstance(element, int):
            for raw_value in self._get_raw_values_iter():
                if element == raw_value:
                    return True
            return False

    def _count_marks_iter(self, *args):
        for arg in args:
            output = 0
            for marks in self._get_raw_marks_iter():
                if arg in marks:
                    output += 1
            yield output

    def count_marks(self, *args) -> tuple:
        return tuple(self._count_marks_iter(*args))

    def index(self, value, reverse:bool=False):
        i = 0 if not reverse else len(self.raw_values) - 1
        step = 1 if not reverse else -1
        for raw_value in self._get_raw_values_iter(reverse):
            if value == raw_value:
                return i
            i += step
        return None

    def validate(self) -> None:
        for i in range(1,10):
            first = self.index(i)
            last = self.index(i, True)

            if first != last and not (first is None or last is None):
                raise Exception("There are more than 1 of {} in this sequence.".format(i))
        



