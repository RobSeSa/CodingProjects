EXECBIN  = lzwcoder

SOURCES  = $(wildcard *.c)
HEADERS  = $(wildcard *.h)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = gcc
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

LINKER   = gcc
LFLAGS   = -Wall -lm

.PHONY: all clean spotless infer debug_test

all: $(EXECBIN) spotless

$(EXECBIN): $(OBJECTS)
	$(LINKER) $? $(LFLAGS) -o $@

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: spotless
	rm -rf mytest.zipped mytest.unzipped
	rm -rf $(EXECBIN) infer-out

spotless:
	rm -rf $(OBJECTS)

infer: spotless
	infer-capture -- make
	infer-analyze -- make

debug_test: all
	./lzwcoder -v -c -i test.txt -o test.zipped
	./lzwcoder -v -d -i test.zipped -o test.unzipped
	@diff test.txt test.unzipped -s

test:
	rm -rf mytest.zipped mytest.unzipped mytest.txt
	ls
	make all
	./lzwcoder -i mytest -o mytest.txt -x
	./lzwcoder -v -c -i mytest.txt -o mytest.zipped
	./lzwcoder -v -d -i mytest.zipped -o mytest.unzipped
	@diff mytest.txt mytest.unzipped -s
	@cmp mytest.txt mytest.unzipped

git-% :
	git add ${SOURCES} ${HEADERS} Makefile
	git commit -m "$(@:git-%=%)"
	git push
