CFLAGS=-std=c++11 -ggdb
main: main.cpp cpu_hardware.cpp
	g++ $(CFLAGS) cpu_hardware.cpp main.cpp -o out
.PHONY: test
test: test/test.cpp cpu_hardware.cpp
	g++ $(CFLAGS) cpu_hardware.cpp test/test.cpp -o out
	./out
