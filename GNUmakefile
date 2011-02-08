# http://aegis.sourceforge.net/auug97.pdf
# rule: dependency
# $@: $<

CC	:= gcc 
CFLAGS	:= -g -Wall -Werror
TARGETS	:= showHandler libULT.a alarmHelper parseUcontext signalme test
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

handin: handin.tar
	echo "Turning in handin.tar containing the following files:"
	tar tf handin.tar
	$(TURNIN) --submit $(GRADER) $(LABNAME) handin.tar

handin.tar: clean
	tar cf handin.tar `find . -type f | grep -v '^\.*$$' | grep -v '/CVS/' | grep -v '/\.svn/' | grep -v '/\.git/' | grep -v 'lab[0-9].*\.tar\.gz' | grep -v '/\~/' | grep -v 'proj-ULT.html' | grep -v '/\.tar/'` 

libULT.a: $(ULT_OBJS)
	ar rcs $@ $(ULT_OBJS)

utility.o: utility.c utility.h
	gcc -c $(CFLAGS) -o $@ $<

thread.o: thread.c thread.h
	gcc -c $(CFLAGS) -o $@ $<

threadList.o: threadList.c threadList.h
	gcc -c $(CFLAGS) -o $@ $<

ULT.o: ULT.c ULT.h
	gcc -c $(CFLAGS) -o $@ $<

interrupt.o: interrupt.c interrupt.h
	gcc -c $(CFLAGS) -o $@ $<

alarmHelper: alarmHelper.c
	gcc $(CFLAGS) -o $@ $<

parseUcontext: parseUcontext.c 
	gcc $(CFLAGS) -o $@ $<

signalme: signalme.c 
	gcc $(CFLAGS) -o $@ $<

showHandler.o: showHandler.c interrupt.h libULT.a $(MOREH)
	gcc -c $(CFLAGS) -o $@ $<

showHandler: showHandler.o libULT.a
	gcc $(CFLAGS) -o $@ $< $(ULT_LIB)

test.o: test.c testULT.h interrupt.h libULT.a $(MOREH)
	gcc -c $(CFLAGS) -o $@ $< ULT.o

test: test.o libULT.a testULT.o
	gcc $(CFLAGS) -o $@ $< testULT.o $(ULT_LIB)
