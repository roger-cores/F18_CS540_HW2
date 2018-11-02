CPPFLAGS= -Wall -Wextra -pedantic

all:
	# Insert command to compile the code and generate executable
	g++ -g -std=c++11 Map.hpp
	g++ -g -std=c++11 test.cpp -o test
	g++ -g -std=c++11 test-scaling.cpp -o testsc
	g++ -g -std=c++11 morseex.cpp -o morseex
	g++ -g -std=c++11 minimal.cpp -o minimal
	g++ -g -std=c++11 test-kec.cpp -o testk
run-test:
	./test
run-minimal:
	./minimal
run-morseex:
	./morseex
run-testsc:
	./testsc
run-testk:
	./testk
checkmem-test:
	valgrind --tool=memcheck --leak-check=full ./test
checkmem-testsc:
	valgrind --tool=memcheck --leak-check=full ./testsc
checkmem-minimal:
	valgrind --tool=memcheck --leak-check=full ./minimal
checkmem-morseex:
	valgrind --tool=memcheck --leak-check=full ./morseex
checkmem-testk:
	valgrind --tool=memcheck --leak-check=full ./testk

clean:
	# Insert command to clean object files, executables
	rm test testsc morseex minimal Map.hpp.gch testk
