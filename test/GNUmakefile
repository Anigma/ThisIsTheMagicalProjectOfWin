# http://aegis.sourceforge.net/auug97.pdf
# rule: dependency
# $@: $<

CC	:= gcc 
CFLAGS	:= -g -Wall -Werror
TARGETS	:= test
TURNIN	:= /lusr/bin/turnin
GRADER	:= muqeet
LABNAME	:= handin-372-labULT
MOREH	:= cfuncproto.h
ULT_SRC	:= ULT.c interrupt.c Thread.c ThreadList.c utility.c
ULT_OBJS := $(ULT_SRC:.c=.o)
ULT_LIB	:=  -L . -l ULT 

all: $(TARGETS)

clean:
	rm -rf core *.o *.out $(TARGETS) *~ *.bak *.orig *.gch

test.o: test.c testULT.h interrupt.h libULT.a $(MOREH)
	gcc -c $(CFLAGS) -o $@ $< ULT.o

test: test.o libULT.a testULT.o
	gcc $(CFLAGS) -o $@ test.o testULT.o $(ULT_LIB)
