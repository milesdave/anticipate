EXE = anticipate

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LDLIBS = -lncurses

RM = rm -f
CP = cp -p
PREFIX = /usr/local/bin

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: LDFLAGS += -s
all: $(EXE)

debug: CFLAGS += -g
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

depend: .depend

.depend: $(SRC)
	$(CC) $(CFLAGS) -MM $^ > $@

include .depend

install:
	$(CP) $(EXE) $(PREFIX)

uninstall:
	$(RM) $(PREFIX)/$(EXE)

clean:
	$(RM) $(OBJ) $(EXE)

.PHONY: clean install uninstall
