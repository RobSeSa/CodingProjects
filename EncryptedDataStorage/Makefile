SUBMITFILES = Makefile asgn4.txt protectfile.c cryptoFS.c test.txt test_read.c basetest.txt
README = README.Captain DESIGN.pdf WRITEUP.pdf README.kvkrishn README.pnampoot
CFLAGS = -Wall -Werror -Wextra -pedantic -O3 -Wshadow

all: protectfile 

protectfile: protectfile.o
	cc -o protectfile protectfile.o

protectfile.o: protectfile.c
	cc -c protectfile.c

e: protectfile
	./protectfile -e d34db00f test.txt -d
	cat test.txt
	printf "\n"

d: protectfile
	./protectfile -d d34db00f test.txt -d
	cat test.txt
	printf "\n"

final:
	make spotless
	make read
	make e
	make mount
	@echo "---------------------------------------------------------------"
	@echo "cd into mountdir and find this same directory and run make test"
	@echo "---------------------------------------------------------------"

refinal:
	make spotless
	make read
	make e
	make remount
	@echo "---------------------------------------------------------------"
	@echo "cd into mountdir and find this same directory and run make test"
	@echo "---------------------------------------------------------------"

test:
	./testread test.txt
	@echo "------------------------------------------"
	@echo "Testing reading and writing"
	@echo ""
	@echo "------------------------------------------"
	chmod +t test.txt
	@echo "test.txt output with encryption bit ON:"
	@echo "------------------------------------------"
	@cat test.txt
	@echo ""
	@echo "------------------------------------------"
	chmod -t test.txt
	@echo "test.txt output with encryption bit OFF:"
	@echo "------------------------------------------"
	@cat test.txt
	@echo ""
	@echo "------------------------------------------"
	chmod -t output.txt
	@echo "output.txt output with encryption bit OFF:"
	@echo "------------------------------------------"
	@cat output.txt
	@echo ""
	@echo "------------------------------------------"
	chmod +t output.txt
	@echo "output.txt output with encryption bit ON:"
	@echo "------------------------------------------"
	@cat output.txt
	@echo ""
	@echo "------------------------------------------"

read: testread 
	echo "" > output.txt
	chmod +t output.txt
	./testread test.txt
	cat output.txt
	@echo ""
testread: test_read.o
	cc -o testread test_read.o
test_read.o : test_read.c
	cc -c test_read.c

remake:
	rm -fr test.txt
	cp basetest.txt test.txt

test-protect : protectfile
	# *****************
	# BASE FILE:
	# *****************
	@echo ""
	@cat test.txt
	./protectfile -e d34db00f test.txt
	@echo ""
	# *****************
	# ENCRYPTED FILE:
	# *****************
	@echo ""
	@cat test.txt
	@echo ""
	./protectfile -d d34db00f test.txt
	@echo ""
	# *****************
	# DECRYPTED FILE:
	# *****************
	@echo ""
	@cat test.txt
	@echo ""

clean:
	rm -fr *.o *.dSYM 
	rm -fr protectfile testread shift output.txt

spotless:
	make clean
	-umount mountdir
	rm -fr mountdir

git-% :
	git add ${SUBMITFILES} ${README} 
	git add sysproto.h vfs_syscalls.c syscalls.master
	git commit -m "$(@:git-%=%)"
	git push

move-fs : cryptoFS.c
	rm -fr ~/cryptoFS.c
	rm -fr ~/cryptoFS.c
	cp cryptoFS.c ~/

cryptoFS :
	make move-fs
	cd ~; cc cryptoFS.c -o cryptoFS `pkgconf fuse --cflags --libs` -o use_ino; sudo ./cryptoFS

mount :
	cd ~; sudo kldload fuse
	make cryptoFS
	mkdir mountdir
	~/cryptoFS mountdir

remount :
	make cryptoFS
	mkdir mountdir
	~/cryptoFS mountdir

move-yourFile.c :
	#rm -f /usr/src/sys/vm/vm_pageout.c
	#cp vm_pageout.c /usr/src/sys/vm/ 

build : yourFile.c
	# make move-yourFile.c
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Done building kernel with yourFile.c changed"
