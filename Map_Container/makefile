CPPFLAGS= -Wall -Wextra -pedantic

all:
	# Insert command to compile the code and generate executable
	g++ -g -std=c++11 Map.hpp
	g++ -g -std=c++11 test.cpp -o test
	g++ -g -std=c++11 test-scaling.cpp -o testsc
	g++ -g -std=c++11 morseex.cpp -o morseex
run:
	# Insert command to run the executable
	./test
	./testsc
	./morseex
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./test
clean:
	# Insert command to clean object files, executables
	rm test testsc morseex
