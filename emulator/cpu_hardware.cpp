#include "cpu_hardware.hpp"
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
unsigned char Memory::getChar(short addr, short offset){
    offset=offset<<12;
    offset=offset>>12;
    
    //placing offset
    int address=offset;
    address<<16;
    address+=addr;
    if(address<MEM_SIZE){
       return _data[address];
    }
    return 0;
}
void Memory::setChar(short addr, short offset,unsigned char to_set){
    //clearing upper 12 bits
    offset=offset<<12;
    offset=offset>>12;
    
    //placing offset
    int address=offset;
    address<<16;
    address+=addr;
    if(address<MEM_SIZE){
       _data[address]=to_set;
    }

}

unsigned short Memory::getShort(short addr, short offset){
    unsigned short out=getChar(addr,offset);
    //shifting to left
    out=out<<8;
    out+=getChar(addr+1,offset);
    return out;
}
void Memory::setShort(short addr, short offset,unsigned short to_set){
    unsigned char front_half=to_set>>8;
    unsigned char back_half=to_set;
    setChar(addr,offset,front_half);
    setChar(addr+1,offset,back_half);
};
void Cpu::run_program(){
    int address=0;
    while(0==0){
        switch (this->program[address].inst)
        {
        case TERM:
            printf("program terminated\n");
            return;
            break;
        case PUSH:
            printf("pushed onto stack\n");
            _sp++;
            _ram.setShort(_sp,_of,getRegister(program[address].arg1));
            address++;
            break;
        case POP:
            printf("poped data\n");
            setRegister(program[address].arg1,_ram.getShort(_sp,_of));
            address++;
            break;
        case MOVE:
            char access_mem_arg1 = program[address].arg1>>3;
            char access_mem_arg2 = program[address].arg2>>3;

            short arg2_data;
            if(access_mem_arg2==1){
                arg2_data=_ram.getShort(getRegister(program[address].arg2),_of);
            }
            if(access_mem_arg2==0){
                arg2_data=getRegister(program[address].arg2);
            }

            if(access_mem_arg1==1){
                _ram.setShort(getRegister(program[address].arg1),_of,arg2_data);
            }
            if(access_mem_arg1==0){
                setRegister(program[address].arg1,arg2_data);
            }
            printf("moved\n");
            break;
        case MOVC:
            printf("movc'd\n");
            char access_mem_arg1 = program[address].arg1>>3;
            short data=program[address].arg2<<8;
            data+=program[address].arg3;
            if(access_mem_arg1==0){
                setRegister(program[address].arg1,data);
            }
            if(access_mem_arg1==1){
                _ram.setShort(getRegister(program[address].arg1),_of,data);
            }
            break;
        default:
            break;
        }
    }
}
void Cpu::setRegister(char register_code,short data){
    switch(register_code){
        case RA:
            _ra=data;
            break;
        case RB:
            _rb=data;
            break;
        case RC:
            _rc=data;
            break;
        case RD:
            _rd=data;
            break;
        case SP:
            _sp=data;
            break;
        case OF:
            _of=data;
            break;
        case IP:
            _ip=data;
            break;
        default:
            printf("should have never reached here!");
            break;
    }
}
unsigned short Cpu::getRegister(char register_code){
    switch(register_code){
        case RA:
            return _ra;
            break;
        case RB:
            return _rb;
            break;
        case RC:
            return _rc;
            break;
        case RD:
            return _rd;
            break;
        case SP:
            return _sp;
            break;
        case OF:
            return _of;
            break;
        case IP:
            return _ip;
            break;
        default:
            printf("should have never reached here!");
            break;
    }
}
Cpu::Cpu(std::string rom_file){
    std::vector<unsigned char> rom = loadFile(rom_file);
    this->program=tokenize(rom);
    run_program();
}