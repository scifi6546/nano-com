#include <stdio.h>
#include "assembler.h"
int main(){
    char * temp = "push ra\npop ra\njump 01h";
    struct string program = makeBin(temp);
    for(int i=0;i<program.str_len;i++){
        printf("%x ",program.str[i]);
    }
}