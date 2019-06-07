#ifndef CPU_HARD_HPP
#define CPU_HARD_HPP
#include <vector>
#include <string>
#include "terminal.hpp"
enum INSTRUCTIONS{TERM=0x00,PUSH=0x01,POP=0x02,MOVE=0x03,MOVC=0x04,
                JUMP=0x05,CALL=0x06,RET=0x07,ADDU=0x08,
                ADDS=0x09,SUB=0x0A,JE=0x0B,JG=0x0C,JL=0x0D,INT=0x0E,INTR=0x0F};
enum REGISTER{RA=0x0,RB=0x1,RC=0x2,RD=0x3,SP=0x4,OF=0x5,IP=0x6};
struct Token{
    INSTRUCTIONS inst=TERM;
    unsigned char arg1=0x0;
    unsigned char arg2=0x0;
    unsigned char arg3=0x0;
    int address=0x0;
};
class Memory{
    public:
        unsigned char getChar(short data, short offset);
        void setChar(short addr, short offset,unsigned char to_set);

        unsigned short getShort(short addr, short offset);
        void setShort(short addr, short offset,unsigned short to_set);
    private:
        const static int MEM_SIZE=0x100000;
        unsigned char _data[MEM_SIZE]={0};
};
class Cpu{
    enum REGISTERS{RA=0x00,RB=0x01,RC=0x02,RD=0x03,SP=0x04,OF=0x05,IP=0x06};
    public:
        Cpu(std::string rom_file_name);

        //starts cpu using a rom used for testing
        Cpu(std::vector<unsigned char> rom);
        //starts error handling routine
        void regError();

        // gets registers at end of program execution
        // for use in testing only
        std::vector<unsigned short> getRegisters();
    private:
        void run_program();
        //sets data in register
        void setRegister(char register_code,short data);
        //gets data from register
        unsigned short getRegister(char register_code);
    //registers
        unsigned short _ra=0;
        unsigned short _rb=0;
        unsigned short _rc=0;
        unsigned short _rd=0;
        unsigned short _sp=0;
        unsigned short _of=0;
        unsigned short _ip=0;
        Memory _ram;
        std::vector<Token> program;

        //runs push instruction
        void push(char register_code);
        //pushes value onto stacl
        void pushValue(unsigned short value);
        //runs pop instruction
        void pop(char register_code);
        //pops value off of stack and returns short
        unsigned short popValue();

        //runs hardware interrupt
        void _int(unsigned short int_code,unsigned short arg1,
            unsigned short arg2,unsigned short arg3,unsigned short arg4);

        //generates a register code from a 1 byte containing two registers
        std::vector<unsigned char> splitRegCode(unsigned char reg_byte);
};
//loads hex data of file
std::vector<unsigned char> loadFile(std::string file_name);
//tokenizes the rom file to make it easy to access
std::vector<Token> tokenize(std::vector<unsigned char> rom);
void printProgram(std::vector<Token> program);
void run_program(std::vector<Token> program);
#endif