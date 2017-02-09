from time import time

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
    iterations = 100
    l = list(range(100)) 
    a = time()
    sort(l)
    b = time()
    print((b - a) / iterations)

if __name__ == "__main__":
    main()
