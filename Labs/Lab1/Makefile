BIN_FILES  = p1 p2 p3 p4 p5 p6

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
#LDLIBS = -lpthread -lm


all: $(BIN_FILES)
.PHONY : all


p1: p1.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

p2: p2.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

p3: p3.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

p4: p4.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

p5: p5.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

p6: p6.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@


%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
