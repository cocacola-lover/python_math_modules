from multiprocessing.sharedctypes import Value
from vector import *
from matrix import *


def methodOfSimpleIteration(A, b, e):
    if (isinstance(A, MyMatrix) and isinstance(b, MyVector) and isinstance(e, float)):
        if (A.checkSquare()):

            dif_criteria = False
            An, bn = A.copy(), b.copy()
            c, B = 0, 0

            for i in range(2):

                u = 1/(An.normInf()); 
                B = MyMatrix.eyeMatrix(An._lines) - An*u; 
                c = bn*u

                if B.normInf() >= 1 and i == 0:
                    An, bn = An.transpon() * An, An.transpon() * bn
                    continue
                if B.normInf() >= 1:
                    dif_criteria = True
                break

            x, k = c, 0

            while(True):
                k += 1
                xNew = B*x + c
                
                if (dif_criteria == False):
                    criteria = (B.normInf() / (1 - B.normInf())) * (xNew - x).normInf()
                else:
                    criteria = abs((A*x - b).normInf())
                if criteria < e: return xNew, k
                x = xNew
    raise ValueError

def zandaleMethod(A, b, e, first = True):
    if (isinstance(A, MyMatrix) and isinstance(b, MyVector) and isinstance(e, float)):
        if (A.checkSquare()):
            C, d = MyMatrix.zeroMatrix(A._lines), MyVector.zeroVector(A._lines)

            An, bn = MyMatrix.makeDiagonalNotZero(A, b)

            for i in range(An._lines):
                d[i] = bn[i] / An[i][i]
                for j in range(An._lines):
                    if i != j:
                        C[i][j] = -An[i][j]/An[i][i]

            x, k = d.copy(), 0
            pre = abs((A*x - b).normInf())

            # Вписать oldA, oldb

            while(True):
                k+=1
                for i in range(C._lines):
                    x[i] = MyVector.termByTermMultiplicationSum(C[i], x) + d[i]
                #x = C*x + d
                
                delta = abs((A*x - b).normInf())
                if (delta < e): 
                    return x, k
                if (delta > pre):
                    if first == False: continue
                    x, k= zandaleMethod(A.transpon()*A, A.transpon()*b, e, False)
                    return x, k
    raise ValueError

