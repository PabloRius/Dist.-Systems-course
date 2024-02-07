BIN_FILES  = p1

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
#LDLIBS = -lpthread -lm


all: $(BIN_FILES)
.PHONY : all


p1: p1.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@


%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
