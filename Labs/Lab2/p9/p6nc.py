import threading
import random
import time
import math

N = 1E7
D = 1E-7


def main():
    result = 0
    sum = 0
    t1 = time.time()

    for i in range(int(N)):
        x = D*i
        result += math.sqrt(4*(1-x*x))

    t2 = time.time()

    t = t2 - t1

    print("El tiempo es: {:.2f}".format(t))

    pi = D*2*result

    print(f"PI={pi}")


if __name__ == "__main__":
    main()
