OUT = rshell
CC = g++
CFLAGS = -Wall -Werror -pedantic -ansi
ODIR = bin
SDIR = src
INC = -Iinc

_OBJS = main.o Processes.o Base.o Command.o Andand.o \
    	Oror.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: $(ODIR)/rshell

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(ODIR)
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

$(ODIR)/(OUT): $(OBJS)
	ar rvs $(OUT) $^

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OUT)