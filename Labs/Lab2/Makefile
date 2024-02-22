BIN_FILES  = p1 p2 p3 p4 p5 p6 p6nc p7

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


all: $(BIN_FILES)
.PHONY : all


p1: p1.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

p2: p2.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

p3: p3.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

p4: p4.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

p5: p5.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

p6: p6.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@ -lm

p6nc: p6nc.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@ -lm

p7: p7.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
