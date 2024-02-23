import threading
import random
import time

NUM_THREADS = 5
ITER = 5
RAND_MAX = 2

lock = threading.Lock()
cond = threading.Condition()


def funcion(id):
    for i in range(ITER):
        with lock:
            k = random.randint(1, RAND_MAX)
            time.sleep(k)
            print(f"Ejecuta el thread {id} la iteracion {i}")


def main():
    threads = []

    for i in range(NUM_THREADS):
        thread = threading.Thread(target=funcion, args=(i,))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()


if __name__ == "__main__":
    main()
