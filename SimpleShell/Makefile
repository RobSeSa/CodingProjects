SUBMITFILES   = myshell.c lex.l Makefile README DESIGN.pdf
CFLAGS = -Wall -Werror -Wextra -pedantic -O3 -Wshadow

all: lex.yy.o myshell.o
	cc -o myshell myshell.o lex.yy.o -lfl

mac : lex.yy.o myshell.o
	cc -o myshell myshell.o lex.yy.o -ll

lex.yy.c : lex.l
	flex lex.l

lex.yy.o : lex.yy.c
	cc -c lex.yy.c

myshell.o : myshell.c
	cc -c ${CFLAGS} myshell.c

infer :
	make clean; infer-capture -- make; infer-analyze -- make 

infer-mac :
	make clean; infer-capture -- make mac; infer-analyze -- make mac

clean:
	rm -fr *.o myshell lex.yy.c pipes infer-out *.dSYM 

run: myshell
	./myshell
	make clean

add :
	git add ${SUBMITFILES}

push :
	git push
	git log
