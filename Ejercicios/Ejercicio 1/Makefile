CC = gcc
CPPFLAGS = -I$(INSTALL_PATH)/include -Wall 
CFLAGS = -lrt
LDFLAGS = -L$(INSTALL_PATH)/lib/

all: servidor cliente

servidor: servidor.o ABB.o utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

cliente: cliente.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f ABB.o servidor.o servidor utils.o cliente.o cliente

.PHONY: all clean
