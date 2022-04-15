import matrix

class MyVector:
    def __init__(self, arr) -> None:
        for i in range(len(arr)):
            if (not isinstance(arr[i], int) and not isinstance(arr[i], float)): raise ValueError

        self._size = len(arr)
        self._arr = arr.copy()

    @staticmethod
    def termByTermMultiplication(a, b):
        if (not isinstance(a, MyVector) or not isinstance(b, MyVector)): raise ValueError
        if (a._size != b._size): raise ValueError

        return MyVector([a[i]*b[i] for i in range(a._size)])

    @staticmethod
    def termByTermMultiplicationSum(a, b):
        return sum(MyVector.termByTermMultiplication(a, b)._arr)

    @staticmethod
    def zeroVector(size):
        return MyVector([0 for i in range(size)])

    def __getitem__(self, item):
        if (isinstance(item, int)):
            return self._arr[item]
        else:
            raise ValueError

    def __setitem__(self, key, value):
        if (isinstance(key, int) and (isinstance(value, float) or isinstance(value, int))):
            self._arr[key] = value
        else:
            raise ValueError
   
    def __mul__(self, other):
        newVector = MyVector(self._arr)
        if (isinstance(other, int) or isinstance(other, float)):
            for i in range(newVector._size): newVector[i] *= other
            return newVector
        elif(isinstance(other, matrix.MyMatrix)):
            if (1 != other._lines): raise ValueError
            
            newMatrix = matrix.MyMatrix.zeroMatrix(self._size, other._columns)
            for i in range(self._size):
                for j in range(other._columns):
                    newMatrix[i][j] = self[i] * other[0][j]
            
            return newMatrix
        else:
            raise ValueError
    
    def __add__(self, other):
        newVector = MyVector(self._arr)
        if (isinstance(other, MyVector)):
            if (self._size == other._size):
                for i in range(self._size): newVector[i] += other[i]
                return newVector
        raise ValueError

    def __sub__(self, other):
        newVector = MyVector(self._arr)
        if (isinstance(other, MyVector)):
            if (self._size == other._size):
                for i in range(self._size): newVector[i] -= other[i]
                return newVector
        raise ValueError

    def __truediv__(self, other):
        newVector = MyVector(self._arr)
        if (isinstance(other, int) or isinstance(other, float)):
            for i in range(newVector._size): newVector[i] /= other
            return newVector
        else:
            raise ValueError
        
    def __str__(self) -> str:
        return "MyVector({})".format(self._arr.__str__())

    def copy(self):
        return MyVector(self._arr)

    def tolist(self):
        return self._arr.copy()
    
    def normInf(self):
        if (len(self._arr) == 0): return 0
        max = abs(self[0])
        for i in range(self._size):
            if (abs(self[i]) > max): max = abs(self[i])
        return max

    def transpon(self):
        return matrix.MyMatrix([[self[i] for i in range(self._size)]])
        

