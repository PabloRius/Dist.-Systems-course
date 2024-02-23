import threading
import argparse
import os

BUFF_SIZE = 10

buffer = ['']*BUFF_SIZE
n_elementos = 0
fin = 0

lleno = threading.Condition()
vacio = threading.Condition()
mutex = threading.Lock()


def productor(fd):
    global buffer, n_elementos, fin
    pos = 0
    while True:
        while n_elementos == BUFF_SIZE:
            with lleno:
                lleno.wait()
        char = os.read(fd, 1)
        if not char:
            fin = 1
            with vacio:
                vacio.notify()
            break

        with mutex:

            buffer[pos] = char.decode()
            n_elementos += 1
            if (n_elementos >= 1):
                with vacio:
                    vacio.notify()

        pos = (pos + 1) % BUFF_SIZE


def consumidor(fd):
    global buffer, n_elementos, fin
    pos = 0

    while True:
        while (n_elementos == 0) and (fin == 0):
            with vacio:
                vacio.wait()
        if n_elementos == 0 and fin == 1:
            break

        with mutex:
            char = buffer[pos]
            n_elementos -= 1

            if n_elementos <= BUFF_SIZE - 1:
                with lleno:
                    lleno.notify()

        pos = (pos + 1) % BUFF_SIZE
        os.write(fd, char.encode())


def main(file_in, file_out):

    with open(file_in, "r") as fin, open(file_out, "w") as fout:

        fdin = fin.fileno()
        fdout = fout.fileno()

        threads = [
            threading.Thread(
                target=consumidor,
                args=(fdout,)
            ),
            threading.Thread(
                target=productor,
                args=(fdin,)
            )
        ]

        for thread in threads:
            thread.start()

        for thread in threads:
            thread.join()


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Programa productor/consumidor con hilos en python")

    parser.add_argument("file_in", type=str,
                        help="Fichero de entrada para el hilo consumidor")
    parser.add_argument("file_out", type=str,
                        help="Fichero de salida para el hilo productor")

    args = parser.parse_args()

    main(args.file_in, args.file_out)
