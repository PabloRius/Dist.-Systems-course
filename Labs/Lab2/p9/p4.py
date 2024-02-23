import threading
import random
import time

NUM_THREADS = 10
ITER = 3
RAND_MAX = 2

cond = threading.Condition()

current_thread = 0


def funcion(id):
    global current_thread
    for i in range(ITER):
        while id != current_thread:
            with cond:
                cond.wait()
        k = random.randint(1, RAND_MAX)
        time.sleep(k)
        print(f"Ejecuta el thread {id} la iteracion {i}")
        current_thread = (current_thread + 1) % NUM_THREADS
        with cond:
            cond.notify_all()


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
