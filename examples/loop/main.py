from time import time

if __name__ == '__main__':
    a = time()
    l = []
    for i in range(1000):
        l = [i]
    b = time()
    print(b-a)
    print(l[0])

