SERVIDOR = servidor

SVD_OBJECTS = servidor.o ABB.o utils.o comm.o

TARGETS = $(SERVIDOR)

CC = gcc
CPPFLAGS = -I$(INSTALL_PATH)/include -Wall 
CFLAGS = -lrt

all: $(TARGETS)

$(SERVIDOR): $(SVD_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

clean:
	rm -f servidor cliente *.o lib/*.so

.PHONY: all clean
