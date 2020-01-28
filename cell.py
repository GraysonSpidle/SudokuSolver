class Cell:
    def __init__(self, value:int=0, marks:list=None):
        self.marks = [] if marks is None else marks
        self.value = value

    def __eq__(self, another) -> bool:
        if isinstance(another, Cell):
            return another.value == self.value and another.marks == self.marks
            
    def __setattr__(self, name, value):
        if name == "value":
            if value != 0:
                self.marks.clear()
        super.__setattr__(self, name, value)

    def mark(self, *args) -> None:
        for arg in args:
            if arg not in self.marks:
                self.marks.append(arg)
    
    def unmark(self, *args) -> None:
        for arg in args:
            if arg in self.marks:
                self.marks.remove(arg)

    @property
    def isBlank(self) -> bool:
        return self.value == 0

    

    


        


