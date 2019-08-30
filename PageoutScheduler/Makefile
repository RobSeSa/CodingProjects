SUBMITFILES = vm_page.c vm_pageout.c Makefile get_data.perl MYKERNEL vm_phys.c WRITEUP.pdf assgn3.txt
DATA = data.txt 
README = README.Captain README.kvkrishn README.pnampoot
CFLAGS = -Wall -Werror -Wextra -pedantic -O3 -Wshadow

stress : 
	stress --cpu 2 --io 1 --vm 1 --vm-bytes 128M --timeout 10s --verbose

install-stress :
	fetch https://people.seas.harvard.edu/~apw/stress/stress-1.0.4.tar.gz
	tar -xf stress-1.0.4.tar.gz
	cd ./stress-1.0.4; ./configure && make && make install\

test :
	echo -e "First data set\n"
	perl get_data.perl >> data.txt
	make stress
	echo -e "Second data set\n" >> data.txt
	perl get_data.perl >> data.txt
	cat data.txt

data : get_data.perl
	perl get_data.perl -v > data.txt
	cat data.txt

clean:
	rm -fr *.o *.dSYM 
	rm -fr ${DATA}

git-% :
	git add ${SUBMITFILES} ${README} 
	git commit -m "$(@:git-%=%)"
	git push

move-vm_phys.c :
	rm -f /usr/src/sys/vm/vm_phys.c
	cp vm_phys.c /usr/src/sys/vm/ 

move-vm_page.c :
	rm -f /usr/src/sys/vm/vm_page.c
	cp vm_page.c /usr/src/sys/vm/ 

move-vm_pageout.c :
	rm -f /usr/src/sys/vm/vm_pageout.c
	cp vm_pageout.c /usr/src/sys/vm/ 

build : vm_phys.c vm_page.c vm_pageout.c
	make move-vm_phys.c
	make move-vm_page.c
	make move-vm_pageout.c
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Done building kernel with vm_phys/page/pageout/.c changed"

rebuild : ./base_files/vm_page.c ./base_files/vm_page.c ./base_files/vm_pageout.c
	rm -f /usr/src/sys/vm/vm_phys.c
	rm -f /usr/src/sys/vm/vm_page.c
	rm -f /usr/src/sys/vm/vm_pageout.c
	cp ./base_files/vm_phys.c /usr/src/sys/vm/ 
	cp ./base_files/vm_page.c /usr/src/sys/vm/ 
	cp ./base_files/vm_pageout.c /usr/src/sys/vm/ 
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Done rebuilding kernel with base vm_phys/page/pageout/.c"

build-full :
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL;\
	make installkernel KERNCONF=MYKERNEL
