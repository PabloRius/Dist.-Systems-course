import threading
import time
import math

N = 1E7
D = 1E-7

NUM_THREADS = 10

total = 0

mutex = threading.Lock()


def calc_thread(id):
    result = 0

    for i in range(id, int(N), NUM_THREADS):
        x = D*i
        result += math.sqrt(4*(1-x*x))

    global total
    with mutex:
        total += result


def main():
    threads = []
    t1 = time.time()

    for i in range(NUM_THREADS):
        thread = threading.Thread(target=calc_thread, args=(i,))
        threads.append(thread)
        thread.start()

    for i in threads:
        thread.join()

    t2 = time.time()

    t = t2 - t1

    print("El tiempo es: {:.2f}".format(t))

    pi = D*2*total

    print(f"PI={pi}")


if __name__ == "__main__":
    main()
