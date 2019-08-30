SUBMITFILES = kern_switch_FIFO.c kern_switch_pq.c sched_ule_splatter.c sched_ule_pri.c Makefile README DESIGN.pdf benchmarks MYKERNEL
CFLAGS = -Wall -Werror -Wextra -pedantic -O3 -Wshadow

clean:
	rm -fr *.o spookMark benchmark infer-out *.dSYM data_benchmark spookyOutput.txt
	rm -fr kern_switch.c sched_ule.c

git-% :
	git add ${SUBMITFILES} 
	git commit -m "$(@:git-%=%)"
	git push

move-kern_switch_FIFO.c :
	rm -f /sys/kern/kern_switch.c
	cp kern_switch_FIFO.c kern_switch.c
	cp kern_switch.c /sys/kern 

move-kern_switch_pq.c :
	rm -f /sys/kern/kern_switch.c
	cp kern_switch_pq.c kern_switch.c
	cp kern_switch.c /sys/kern 

move-sched_ule_pri.c :
	rm -f /usr/src/sys/kern/sched_ule.c
	cp sched_ule_pri.c sched_ule.c
	cp sched_ule.c /usr/src/sys/kern/sched_ule.c

move-sched_ule_splatter.c :
	rm -f /usr/src/sys/kern/sched_ule.c
	cp sched_ule_splatter.c sched_ule.c
	cp sched_ule.c /usr/src/sys/kern/sched_ule.c

build-1 :
	make move-kern_switch_FIFO.c
	make move-sched_ule_pri.c
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Finished building case 1: FIFO queues and priority runq scheduling"

build-2 :
	make move-kern_switch_pq.c
	make move-sched_ule_pri.c
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Finished building case 2: priority queues and priority runq scheduling"

build-3 :
	make move-kern_switch_FIFO.c
	make move-sched_ule_splatter.c
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Finished building case 3: FIFO queues and splatter runq scheduling"

build-4 :
	make move-kern_switch_pq.c
	make move-sched_ule_splatter.c
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL -DKERNFAST;\
	make installkernel KERNCONF=MYKERNEL
	echo "Finished building case 4: priority queues and splatter runq scheduling"

build-full :
	cd /usr/src; make buildkernel KERNCONF=MYKERNEL;\
	make installkernel KERNCONF=MYKERNEL
