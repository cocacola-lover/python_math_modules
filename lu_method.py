from vector import *
from matrix import *
from preciseMethods import gaussianMethod

def LUMethod(A, b):
        if (not isinstance(A, MyMatrix) or not isinstance(b, MyVector)): raise ValueError
        if (not A.checkSquare()): raise ValueError

        M, P = A.copy(), MyMatrix.eyeMatrix(A._lines)
        
        for i in range(A._lines):
            # Поиск el
            el = M[i][i]
            rem = i
            for j in range(i, A._lines):
                if (abs(M[j][i]) > el):
                    el = M[j][i]
                    rem = j

            #Меняем местами
            if (rem != i):
                M[i], M[rem] = M[rem], M[i]
                P[i], P[rem] = P[rem], P[i]

            # Преобразуем
            for j in range(i+1, A._lines): 
                M[j][i] /= M[i][i]
                for k in range(i+1, A._lines): M[j][k] -= M[j][i] * M[i][k]
        
        M = M + MyMatrix.eyeMatrix(A._lines)
        L, U = MyMatrix.eyeMatrix(A._lines), MyMatrix.zeroMatrix(A._lines)

        for i in range(A._lines): 
            M[i][i] -= 1
            for j in range(0, i): L[i][j] = M[i][j]
            for j in range(i, A._lines): U[i][j] = M[i][j]

        y = gaussianMethod(L, P * b)
        x = gaussianMethod(U, y)

        return x



            



            


