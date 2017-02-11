from time import time

def f(x):
    return x*x-x

def integrate_f(a, b, N):
    s = 0
    dx = (b - a) / N
    for i in range(N):
        s += f(a + i * dx)
    return s * dx

if __name__ == "__main__":
    iterations = 10
    N          = 200
    start      = 1
    ans = 0
    a = time()
    for i in range(iterations):
        ans = integrate_f(start, i, N)
        print(ans)
    b = time()
    print(b - a)

