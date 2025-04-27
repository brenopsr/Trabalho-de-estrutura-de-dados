CC = gcc
CFLAGS = -Wall -std=c99

OBJS = main.o vector.o bst.o avl.o frequency_avl.o

# [Correção] Ajustes para multiplataforma (Windows vs Linux/Mac)
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
    LDLIBS = -lmingw32 -lmsvcrt
else
    EXE_EXT =
    RM = rm -f
    LDLIBS =
endif

EXE = main$(EXE_EXT)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o main.exe main

