# RPC Makefile

CLIENTE = cliente
SERVIDOR = servidor

SOURCE.x = suma.x
OBJECTS_CLNT = suma_clnt.o cliente.o suma_xdr.o
OBJECTS_SVC = suma_svc.o servidor.o suma_xdr.o

TARGETS = suma.h suma_xdr.c suma_clnt.c suma_svc.c

CC = gcc
CPPFLAGS = -I$(INSTALL_PATH)/include -Wall -D_REENTRANT
CFLAGS = -lrt -g -I/usr/include/tirpc
LDFLAGS = -L$(INSTALL_PATH)/lib/
LDIBS =  -Llib -lnsl -lpthread -ldl -ltirpc
# -lclaves

RPCGENFLAGS = -N -M -a

all: $(SERVIDOR) $(CLIENTE)

$(TARGETS): $(SOURCE.x)
	rpcgen $(RPCGENFLAGS) $(SOURCE.x)

$(SERVIDOR): $(OBJECTS_SVC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDIBS) -o $@

$(CLIENTE): $(OBJECTS_CLNT)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $<

clean:
	rm -f servidor cliente *.o lib/*.so suma_client.c suma_server.c $(TARGETS) Makefile.suma

.PHONY: all clean
