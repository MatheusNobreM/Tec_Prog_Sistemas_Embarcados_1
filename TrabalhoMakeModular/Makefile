CC = gcc
CFLAGS = -Wall -g
BINDIR = bin
OBJDIR = obj
SRCDIR = src
INCDIR = src

OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/funcao01.o $(OBJDIR)/funcao02.o $(OBJDIR)/funcao03.o $(OBJDIR)/imprime.o
EXEC = $(BINDIR)/app

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(EXEC)

.PHONY: all clean
