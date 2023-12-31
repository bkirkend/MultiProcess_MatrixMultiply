build:
	gcc -g matrix_mult.c -o test

check:removec
	gcc matrix_check.c -o check

clean:
	rm -f test
	rm -f check
	rm -f *.txt
	rm -f *~
	rm -rf *.dSYM

removec:
	rm -f check

memory:
	leaks --atExit -- ./test

test: check build
	time ./check > checkout.txt
	time ./test > testout.txt
	diff checkout.txt testout.txt
