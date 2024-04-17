CC = gcc
CPPFLAGS = -I$(INSTALL_PATH)/include -Wall 
CFLAGS = -lrt
LDFLAGS = -L$(INSTALL_PATH)/lib/
LIBS = -lclaves -Llib

all: libclaves.so servidor cliente

libclaves.so: claves.o utils.o comm.o
	$(CC) -shared $^ -o lib/$@

servidor: servidor.o ABB.o utils.o comm.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

cliente: cliente.o utils.o comm.o
	$(CC) $(CFLAGS) $^ $(LIBS)  -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

clean:
	rm -f servidor cliente *.o lib/*.so

.PHONY: all clean
