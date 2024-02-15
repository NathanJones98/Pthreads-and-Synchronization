CC = g++ -pthread -fgnu-tm
CFLAGS = -O3
NUM_THREADS = 4
SAMPLES_TO_SKIP = 100

# the run version automatically runs the program, sorts output, and compares to the original output
all: randtrack randtrack_parallelized randtrack_global_lock randtrack_list_lock randtrack_element_lock randtrack_reduction
all-run: randtrack-run randtrack_parallelized-run randtrack_global_lock-run randtrack_list_lock-run randtrack_element_lock-run randtrack_reduction-run

#Original
randtrack: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) randtrack.cc -o randtrack

randtrack-run: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) randtrack.cc -o randtrack

	/usr/bin/time randtrack $(NUM_THREADS) $(SAMPLES_TO_SKIP) > rt.out
	sort -n rt.out > rt.outs


#Original-parallelized
randtrack_parallelized: list.h hash.h defs.h randtrack_parallelized.cc
	$(CC) $(CFLAGS) randtrack_parallelized.cc -o randtrack_parallelized

randtrack_parallelized-run: list.h hash.h defs.h randtrack_parallelized.cc
	$(CC) $(CFLAGS) randtrack_parallelized.cc -o randtrack_parallelized

	/usr/bin/time randtrack_parallelized $(NUM_THREADS) $(SAMPLES_TO_SKIP) > rt_parallelized.out
	sort -n rt_parallelized.out > rt_parallelized.outs


#Global
randtrack_global_lock: list.h hash.h defs.h randtrack_global_lock.cc
	$(CC) $(CFLAGS) randtrack_global_lock.cc -o randtrack_global_lock

randtrack_global_lock-run: list.h hash.h defs.h randtrack_global_lock.cc
	$(CC) $(CFLAGS) randtrack_global_lock.cc -o randtrack_global_lock

	/usr/bin/time randtrack_global_lock $(NUM_THREADS) $(SAMPLES_TO_SKIP) > rt_global.out
	sort -n rt_global.out > rt_global.outs

	diff rt.outs rt_global.outs > diff-rt_global.out


#List
randtrack_list_lock: list.h hash.h defs.h randtrack_list_lock.cc
	$(CC) $(CFLAGS) randtrack_list_lock.cc -o randtrack_list_lock

randtrack_list_lock-run: list.h hash.h defs.h randtrack_list_lock.cc
	$(CC) $(CFLAGS) randtrack_list_lock.cc -o randtrack_list_lock

	/usr/bin/time randtrack_list_lock $(NUM_THREADS) $(SAMPLES_TO_SKIP) > rt_list.out
	sort -n rt_list.out > rt_list.outs

	diff rt.outs rt_list.outs > diff-rt_list.out


#Element
randtrack_element_lock: list_element_lock.h hash_element_lock.h defs.h randtrack_element_lock.cc
	$(CC) $(CFLAGS) randtrack_element_lock.cc -o randtrack_element_lock

randtrack_element_lock-run: list_element_lock.h hash_element_lock.h defs.h randtrack_element_lock.cc
	$(CC) $(CFLAGS) randtrack_element_lock.cc -o randtrack_element_lock

	/usr/bin/time randtrack_element_lock $(NUM_THREADS) $(SAMPLES_TO_SKIP) > rt_element.out
	sort -n rt_element.out > rt_element.outs

	diff rt.outs rt_element.outs > diff-rt_element.out


#Reduction
randtrack_reduction: list.h hash.h defs.h randtrack_reduction.cc
	$(CC) $(CFLAGS) randtrack_reduction.cc -o randtrack_reduction

randtrack_reduction-run: list.h hash.h defs.h randtrack_reduction.cc
	$(CC) $(CFLAGS) randtrack_reduction.cc -o randtrack_reduction

	/usr/bin/time randtrack_reduction $(NUM_THREADS) $(SAMPLES_TO_SKIP) > rt_reduction.out
	sort -n rt_reduction.out > rt_reduction.outs

	diff rt.outs rt_reduction.outs > diff-rt_reduction.out

#ALL
clean:
	rm -f *.o randtrack randtrack_parallelized randtrack_global_lock randtrack_list_lock randtrack_element_lock randtrack_reduction rt.out rt.outs rt_parallelized.out rt_parallelized.outs rt_global.out rt_global.outs rt_list.out rt_list.outs rt_element.out rt_element.outs rt_reduction.out rt_reduction.outs diff-rt_parallelized.out diff-rt_global.out diff-rt_list.out diff-rt_element.out diff-rt_reduction.out

clean-randtrack: 
	rm -f *.o randtrack rt.out rt.outs

clean-randtrack_parallelized: 
	rm -f *.o randtrack_parallelized rt_parallelized.out rt_parallelized.outs diff-rt_parallelized.out

clean-randtrack_global_lock: 
	rm -f *.o randtrack_global_lock rt_global.out rt_global.outs diff-rt_global.out

clean-randtrack_list_lock: 
	rm -f *.o randtrack_list_lock rt_list.out rt_list.outs diff-rt_list.out

clean-randtrack_element_lock: 
	rm -f *.o randtrack_element_lock rt_element.out rt_element.outs diff-rt_element.out

clean-randtrack_reduction: 
	rm -f *.o randtrack_reduction rt_reduction.out rt_reduction.outs diff-rt_reduction.out