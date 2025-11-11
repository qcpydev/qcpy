class Wire:
    """Private handler of array of strings
    Note:
        This is a work in progress and may see some small bugs/invalid formations.
        In other iterations, this will change functionality!
    Attributes:
        length (int): Length of the array of content.
        content (arr): Array of strings that were inserted into by circuit_drawing.
    """

    def __init__(self):
        self.length = 0
        self.content = []

    def add(self, to_add: str) -> None:
        """Appends a string into the content
        Args:
            to_add (str): String to add.
        """

        self.length += 1
        self.content.append(to_add)

    def insert(self, to_insert: int, to_add: str) -> None:
        """Inserts a string into the content array
        Args:
            to_insert (int): Where to insert drawing.
            to_add (str): The string to insert.
        """

        if to_insert >= self.length:
            self.add(to_add)
        else:
            self.content[to_insert] = to_add

    def at(self, index: int) -> str:
        """Returns the string at a certain index
        Args:
            index (int): To try and find the string at a specific location.
        Returns:
            str: empty if nothing was found, else the value stored at the index.
        """

        return "" if index >= self.length else self.content[index]
