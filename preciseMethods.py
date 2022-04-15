from vector import *
from matrix import *
import norms

def gaussianMethod(A, b):
        if (isinstance(A, MyMatrix) and isinstance(b, MyVector)):
            if (A.checkSquare()):
                An, bn = A.copy(), b.copy()

                for j in range(A._lines-1):
                    #Меняем местами, если придётся
                    if (An[j][j] == 0):
                        for i in range(j+1, An._lines):
                            if (An[i][j] != 0):
                                An[j], An[i] = An[i], An[j]
                                bn[j], bn[i] = bn[i], bn[j]
                                break
                        if (An[j][j] == 0): continue
                    #Теперь вычитаем
                    An[j], bn[j] = An[j] / An[j][j], bn[j] / An[j][j]
                    for i in range(j+1, An._lines):
                        bn[i] = bn[i] - bn[j] * An[i][j]
                        An[i] = An[i] - An[j] * An[i][j]


                for j in range(A._lines-1, -1, -1):
                    An[j], bn[j] = An[j] / An[j][j], bn[j] / An[j][j]
                    for i in range(j-1, -1, -1):
                        bn[i] = bn[i] - bn[j] * An[i][j]
                        An[i] = An[i] - An[j] * An[i][j]


                return bn
        raise ValueError

def mirroringMethod(A, b):
        if (isinstance(A, MyMatrix) and isinstance(b, MyVector)):
            if (A.checkSquare()):
                An, bn = A.copy(), b.copy()

                for i in range(An._lines-1):
                    y = MyVector([An[j][i] for j in range(i, An._lines)])
                    z = MyVector([1] + [0 for i in range(i+1, An._lines)])

                    alpha = norms.norm2(y) / norms.norm2(z)

                    w = (y - z * alpha) / norms.norm2(y - z * alpha)
                    U = MyMatrix.eyeMatrix(An._lines - i) - (w * w.transpon() * 2)
                    U = U.raiseDimension(An._lines)

                    An, bn = U*An, U*bn

                for j in range(A._lines-1, -1, -1):
                    An[j], bn[j] = An[j] / An[j][j], bn[j] / An[j][j]
                    for i in range(j-1, -1, -1):
                        bn[i] = bn[i] - bn[j] * An[i][j]
                        An[i] = An[i] - An[j] * An[i][j]

                return bn
        raise ValueError




                    

