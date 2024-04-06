BIN_FILES  = cliente-calc servidor-calc

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/


all: $(BIN_FILES)
.PHONY : all


cliente-calc: cliente-calc.o lines.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

servidor-calc: servidor-calc.o lines.o
	$(CC) $(LDFLAGS) $^  -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
