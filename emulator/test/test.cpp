#include <stdio.h>
#include <gtest/gtest.h>
#include "../cpu_hardware.hpp"
int main(){
	std::vector<unsigned char> test={0x00,0x00,0x00,0x00};
	Cpu test_cpu(test);
	printf("worked\n\n");
	return 0;
}
