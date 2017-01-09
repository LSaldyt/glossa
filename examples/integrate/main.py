def f(x):
    return x*x-x

def integrate_f(a, b, N):
    s = 0
    dx = (b-a)/N
    for i in range(N):
        s = s + f(a + i * dx)
    return s * dx

if __name__ == "__main__":
    print(integrate_f(1, 100, 200))

