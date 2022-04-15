from matrix import * 
from vector import *

def mySqrt(x:float, e = 10**-5):
    sqrt_ = x; added = 1
    while (added > e):
        old_sqrt = sqrt_
        if sqrt_ == 0: return 0
        sqrt_ = 0.5 * (sqrt_ + x/sqrt_)
        added = abs(old_sqrt - sqrt_)
    return sqrt_

def norm1(a):
    if (isinstance(a, MyVector)):
        sum = 0
        for i in a._arr: sum += abs(i)
        return sum
    raise ValueError

def norm2(a):
    if (isinstance(a, MyVector)):
        sum = 0
        for i in a._arr: sum += i**2
        return mySqrt(sum)
    raise ValueError

def normInf(a):
    if (isinstance(a, MyVector)):
        if (len(a._arr) == 0): return 0
        max = abs(a[0])
        for i in range(a._size):
            if (abs(a[i]) > max): max = abs(a[i])
        return max
