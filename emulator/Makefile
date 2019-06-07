CFLAGS=-std=c++11 -ggdb
main: main.cpp cpu_hardware.cpp terminal.cpp
	g++ $(CFLAGS) cpu_hardware.cpp main.cpp terminal.cpp -lncurses -o out
.PHONY: test
test: test/test.cpp cpu_hardware.cpp
	cmake -g "Unix Makefile" test/build/CMakeLists.txt
	make -C test/build
	./test/build/executeTests
