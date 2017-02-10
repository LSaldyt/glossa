from time import time

if __name__ == '__main__':
    a = time()
    for i in range(1000):
        pass
    b = time()
    print(b-a)

