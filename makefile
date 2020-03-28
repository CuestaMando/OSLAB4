CC = gcc
CCOPTS = -c -g -Wall
LINKOPTS = -g
C99 = -std=gnu99

SRCDIR = src
INCDIR = inc
OBJDIR = bin

FILES = memory_mgmt process queue util main
OUT = hostd

OBJS := $(FILES:%=$(OBJDIR)/%.o)
INCS := $(FILES:%=$(INCDIR)/%.h)
SRCS := $(FILES:%=$(SRCDIR)/%.c)

$(OUT): $(OBJS)
	$(CC) $(LINKOPTS) $^ -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(INCDIR)/process_mgmt.h
	$(CC) $(CCOPTS) $(C99) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CCOPTS) $(C99) $< -o $(OBJDIR)/$*.o

clean:
	rm $(OBJDIR)/*
	rmdir $(OBJDIR)