#include <stdio.h>
#include "assembler.h"
int main(){
    char * temp = "push ra\npop ra";
    makeBin(temp);
}