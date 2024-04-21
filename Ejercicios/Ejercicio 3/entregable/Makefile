# RPC Makefile

CLAVES = libclaves.so
CLIENTE = cliente
SERVIDOR = servidor

SOURCE.x = servicio.x
OBJECTS_CLAVES = servicio_clnt.o claves.o servicio_xdr.o
OBJECTS_CLNT = cliente.o utils.o
OBJECTS_SVC = servicio_svc.o servidor.o servicio_xdr.o ABB.o utils.o

TARGETS = servicio.h servicio_xdr.c servicio_clnt.c servicio_svc.c

CC = gcc
CPPFLAGS = -I$(INSTALL_PATH)/include -Wall -D_REENTRANT
CFLAGS = -lrt -g -I/usr/include/tirpc
LDFLAGS = -L$(INSTALL_PATH)/lib/
LDIBS =  -lnsl -lpthread -ldl -ltirpc
LDLIBS_C = -Llib -lclaves

RPCGENFLAGS = -N -M -a

all: $(CLAVES) $(SERVIDOR) $(CLIENTE)

$(TARGETS): $(SOURCE.x)
	rpcgen $(RPCGENFLAGS) $(SOURCE.x)

$(CLAVES): $(OBJECTS_CLAVES)
	$(CC) $(CFLAGS) -shared $^ $(LDFLAGS) $(LDIBS) -o lib/$@

$(SERVIDOR): $(OBJECTS_SVC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDIBS) -o $@

$(CLIENTE): $(OBJECTS_CLNT)
	$(CC) $^ $(LDLIBS_C) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

clean:
	rm -f servidor cliente *.o lib/*.so servicio_client.c servicio_server.c $(TARGETS) Makefile.servicio

.PHONY: all clean
