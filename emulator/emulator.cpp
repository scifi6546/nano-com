#include <vector>
#include <string>
#include <stdio.h>
enum INSTRUCTIONS{TERM=0x00,PUSH=0x01,POP=0x02,MOVE=0x03,
                JUMP=0x05,CALL=0x06,RET=0x07,ADDU=0x08,
                ADDS=0x09,SUB=0x0A};
struct Token{
    INSTRUCTIONS inst=TERM;
    unsigned char arg1=0x0;
    unsigned char arg2=0x0;
    unsigned char arg3=0x0;
    int address=0x0;
};
//loads hex data of file
std::vector<unsigned char> loadFile(std::string file_name){
    FILE *romContents = fopen(file_name.c_str(),"r");
    std::vector<unsigned char> file_contents;
    char temp;
    while(0==0){
        temp=fgetc(romContents);
        if(temp==EOF){
            return file_contents;
        }
        file_contents.push_back(temp);
    }
}
//tokenizes the rom file to make it easy to access
std::vector<Token> tokenize(std::vector<unsigned char> rom){
    if(rom.size()%4!=0){
        printf("ERROR rom size not correct\n");
        return std::vector<Token>{};
    }
    std::vector<Token> program;
    for(int i=0;i<rom.size();i+=4){
        Token temp;
        temp.inst=(INSTRUCTIONS) rom[i];
        temp.arg1=rom[i+1];
        temp.arg2=rom[i+2];
        temp.arg3=rom[i+3];
        program.push_back(temp);
    }
    return program;
}
void printProgram(std::vector<Token> program){
    for(int i =0;i<program.size();i++){
        printf("Address: %i, Instruction: %i, Arg 1: %u, Arg 2: %u, Arg 3: %u\n",
        program[i].address,program[i].inst,program[i].arg1,program[i].arg2,program[i].arg3);
    }
}
int main(){
    std::vector<unsigned char> file = loadFile("rom.nano");
    std::vector<Token> prog= tokenize(file);
    printProgram(prog);
}