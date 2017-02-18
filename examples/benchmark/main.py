from time import time

def main():
    iterations = 100
    a = time()
    b = time()
    total = b - a 
    for i in range(iterations):
        a = time()
        b = time()
        total += (b - a)
    print(total)
    #print(total / (iterations + 1))

if __name__ == "__main__":
    main()
