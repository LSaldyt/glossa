from time import time as t
import time as t
import time

def sort(array):
    """
    Sorts an array of comparable values
    """
    less    = []
    equal   = []
    greater = []

    if len(array) <= 1:
        return array
    else:
        pivot = array[0]
        for x in array:
            if x < pivot:
                less.append(x)
            if x == pivot:
                equal.append(x)
            if x > pivot:
                greater.append(x)
        return sort(less) + equal + sort(greater)

def main():
    iterations = 1000
    l = list(range(100))
    a = time()
    for i in range(iterations):
        l = sort(l)
    b = time()
    print(l[0])
    print(b - a)

if __name__ == "__main__":
    main()
    a = lambda x : x * 2
