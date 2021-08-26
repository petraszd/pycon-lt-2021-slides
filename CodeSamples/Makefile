int_types:
	clang -Wall int_types.c
	./a.out > int_types.c.out
	rm a.out


assembly:
	clang -Wall assembly.c
	./a.out > assembly.c.out
	rm ./a.out

speed_test_c:
	clang -Wall speed_test.c
	(time ./a.out) > temp.txt 2>&1
	cat temp.txt | expand > speed_test.c.out
	rm ./a.out
	rm temp.txt

speed_test_py:
	(time python3 speed_test.py) > temp.txt 2>&1
	cat temp.txt | expand > speed_test.py.out
	rm temp.txt

overflow:
	clang -Wall overflow.c
	./a.out > overflow.c.out
	rm ./a.out
