from time import time

def f(x):
    return x*x-x

def integrate_f(a, b, N):
    s = 0
    dx = (b-a)/N
    for i in range(N):
        s = s + f(a + i * dx)
    return s * dx

if __name__ == "__main__":
    iterations = 1
    N = 200
    start = 1
    r = list(range(iterations))
    a = time()
    for i in r:
        integrate_f(start, i, N)
    b = time()
    print((b - a) / iterations)

