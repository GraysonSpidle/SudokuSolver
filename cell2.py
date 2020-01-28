class Cell:
    def __init__(self, row:int, col:int, value:int=0, marks:list=None):
        self.row = row
        self.col = col
        self.box = ((row // 3) * 3) + (col // 3)
        self.marks = [] if marks is None else marks
        self.value = value

    def __contains__(self, m) -> bool: # It is worth noting, this method only checks the cell's marks.
        if isinstance(m, int):
            return m in self.marks
        elif hasattr(m, "__iter__") or hasattr(m, "__getitem__"): # m is iterable
            return all(mark in self.marks for mark in m)

    def __eq__(self, another) -> bool:
        if isinstance(another, Cell):
            return id(self) == id(another)
        elif isinstance(another, int):
            return another == self.value
            
    def __setattr__(self, name, value):
        if name == "value":
            if value != 0:
                self.marks.clear()
        super.__setattr__(self, name, value)

    def mark(self, *args) -> bool:
        made_changes = False
        if not self.isBlank:
            print("Warning: attempted to mark a non-blank cell")
            return made_changes
        for arg in args:
            if arg not in self.marks:
                self.marks.append(arg)
                made_changes = True
                
        return made_changes
    
    def unmark(self, *args) -> bool:
        made_changes = False
        if not self.isBlank:
            print("Warning: attempted to unmark a non-blank cell")
        for arg in args:
            try:
                self.marks.remove(arg)
                made_changes = True
            except ValueError:
                pass # do nothing
        return made_changes

    @property
    def isBlank(self) -> bool:
        return self.value == 0