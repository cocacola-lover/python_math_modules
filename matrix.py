#from vector import *
import vector

class MyMatrix:

    @staticmethod
    def makeDiagonalNotZero(A, b):
        if (isinstance(A, MyMatrix) and  isinstance(b, vector.MyVector)):
            if (A.checkSquare()):
                check = True
                for i in range(A._lines):
                    if (A[i][i] == 0): check = False
                if check == True: return A, b

                An, bn = A.copy(), b.copy()
                check = []
                for i in range(A._lines):
                    arr = []
                    for j in range(A._lines):
                        if A[i][j] != 0: arr += [j]
                    check += [arr]
                
                check = sorted(check, key = lambda i:len(i))

                for i in range(A._lines):
                    if len(check[i]) == 0: raise ValueError

                    An[i], bn[i] = A[check[i][0]], b[check[i][0]]
                    for j in range(i+1, A._lines):
                        if check[i][0] in check[j]: check[j].remove(check[i][0])
                
                return An, bn



    @staticmethod
    def zeroMatrix(lines, columns = 0):
        if (isinstance(lines, int) and isinstance(columns, int)):
            if columns == 0: return MyMatrix([[0 for i in range(lines)] for i in range(lines)])
            return MyMatrix([[0 for i in range(columns)] for i in range(lines)])
        raise ValueError

    @staticmethod
    def eyeMatrix(size):
        if (isinstance(size, int)):
            ans = MyMatrix.zeroMatrix(size)
            for i in range(size): ans[i][i] = 1
            return ans
        raise ValueError

    def checkSizesAndUniformity(self, arr):
        lines, columns = len(arr), len(arr[0])

        if (not isinstance(arr, list)): raise ValueError
        for i in range(lines):
            if (not isinstance(arr[i], list)): raise ValueError

        for i in range(lines):
            if len(arr[i]) != columns: raise ValueError

        for i in range(lines):
            for j in range(columns):
                if (not isinstance(arr[i][j], int) and not isinstance(arr[i][j], float)): raise ValueError

        return (lines, columns)

    def normInf(self):
        ans = 0

        for i in range(self._lines):
            sum = 0
            for j in range(self._columns): sum += abs(self[i][j])
            if sum > ans: ans = sum

        return ans


    def __init__(self, arr) -> None:
        self._arr = []
        self._lines, self._columns = self.checkSizesAndUniformity(arr)

        for i in range(self._lines):
            self._arr += [vector.MyVector(arr[i])]

    def __str__(self) -> str:
        ans = "MyMatrix:\n"
        for i in range(self._lines):
            ans += " " + self._arr[i].__str__() + "\n"
        ans = ans[:len(ans)-1] + ";"
        return ans
    
    def copy(self):
        newMatrix = MyMatrix([[]])
        newMatrix._arr = []
        for i in range(self._lines):
            newMatrix._arr += [self._arr[i].copy()]
        newMatrix._lines, newMatrix._columns = self._lines, self._columns
        return newMatrix

    def __getitem__(self, item):
        if (isinstance(item, int)):
            return self._arr[item]
        else:
            raise ValueError

    def __setitem__(self, key, value):
        if (isinstance(key, int) and isinstance(value, vector.MyVector)):
            self._arr[key] = value
        else:
            raise ValueError

    def __mul__(self, other):
        if (isinstance(other, vector.MyVector)):
            newVector = vector.MyVector([0 for i in range(self._lines)])
            if (self._columns == other._size):
                for i in range(self._lines):
                    result = 0
                    for j in range(self._columns):
                        result += self[i][j] * other[j]
                    newVector[i] = result
                return newVector
        elif (isinstance(other, int) or isinstance(other, float)):
            newMatrix = self.copy()
            for i in range(self._lines):
                for j in range(self._columns): newMatrix[i][j] *= other
            return newMatrix
        elif (isinstance(other, MyMatrix)):
            if (self._columns != other._lines): raise ValueError
            newMatrix = MyMatrix.zeroMatrix(self._lines, other._columns)

            for i in range(self._lines):
                for j in range(other._columns):
                    newMatrix[i][j] = vector.MyVector.termByTermMultiplicationSum(self.getLine(i), other.getColumn(j))
            
            return newMatrix

        raise ValueError
    
    def __add__(self, other):
        if (isinstance(other, MyMatrix)):
            if (self._lines == other._lines and self._columns == other._columns):
                newMatrix = self.copy()
                for i in range(self._lines):
                    for j in range(self._columns): newMatrix[i][j] += other[i][j]
                return newMatrix
        raise ValueError

    def __sub__(self, other):
        if (isinstance(other, MyMatrix)):
            if (self._lines == other._lines and self._columns == other._columns):
                newMatrix = self.copy()
                for i in range(self._lines):
                    for j in range(self._columns): newMatrix[i][j] -= other[i][j]
                return newMatrix
        raise ValueError

    def checkSquare(self):
        return self._lines == self._columns

    def transpon(self):
        newMatrix = MyMatrix.zeroMatrix(self._columns, self._lines)

        for i in range(self._lines):
            for j in range(self._columns):
                newMatrix[j][i] = self[i][j]
        return newMatrix

    def getColumn(self, key):
        if (not isinstance(key, int)): raise ValueError
        if (key < 0 or key > self._columns-1): raise ValueError

        return vector.MyVector([self[i][key] for i in range(self._lines)])

    def getLine(self, key):
        if (not isinstance(key, int)): raise ValueError
        if (key < 0 or key > self._lines-1): raise ValueError

        return self[key].copy()

    def toVector(self):
        if (self._columns != 1): raise ValueError
        return vector.MyVector([self[i][0] for i in range(self._lines)])

    def raiseDimension(self, n):
        if (not isinstance(n, int) or not self.checkSquare() or n < self._lines): raise ValueError
        if (n == self._lines): return self.copy()

        newMatrix = MyMatrix.eyeMatrix(n)
        for i in range(n - self._lines, n):
            for j in range(n - self._lines, n):
                newMatrix[i][j] = self[i - n + self._lines][j - n + self._lines]
        return newMatrix

    def tolist(self):
        newList = self._arr.copy()

        for i in range(self._lines): newList[i] = newList[i].tolist()
        return newList





