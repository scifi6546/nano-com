#include "cpu_hardware.hpp"
#include <stdio.h>
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
    unsigned int address=0;
    //placing offset
    address+=offset;
    address<<16;
    address+=addr;
    //printf("to_set: %i\n",to_set);
    if(to_set==152){
        printf("to_set not rignt\n");
    }
    if(address<MEM_SIZE){
       _data[address]=to_set;
    }else{
        printf("wrong memory accessed\n");
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
    while(0==0){
        switch (this->program[_ip].inst)
        {
        case TERM:{
            //printf("program terminated\n");
            return;
            break;
        }
        case PUSH:{
            unsigned char reg_code=splitRegCode(program[_ip].arg1)[1];
            push(reg_code);
            //printf("pushed onto stack\n");
            _ip++;
            break;
        }
        case POP:{
            //printf("poped data\n");
            unsigned char reg_code=splitRegCode(program[_ip].arg1)[1];
            pop(reg_code);
            _ip++;
            break;
        }
        case MOVE:{
            std::vector<unsigned char> reg_codes=splitRegCode(program[_ip].arg1);
            unsigned char dest = reg_codes[0];
            unsigned char src = reg_codes[1];
            unsigned short src_value = getRegister(src);
            setRegister(dest,src_value); 
            _ip++;
            break;
        }
        case MOVC:{
            //printf("movc'd\n");
            unsigned char dest_reg=splitRegCode(program[_ip].arg1)[1];
            unsigned short data=program[_ip].arg2<<8;
            data+=program[_ip].arg3;
            setRegister(dest_reg,data);
            _ip++;
            break;
        }
        case JUMP:{
            unsigned short addr=program[_ip].arg1<<8;
            addr+=program[_ip].arg2;
            _ip=addr;
            //printf("jumped\n");
            break;
        }
        case CALL:{
            pushValue(_ip+1);
            unsigned short addr=program[_ip].arg1<<8;
            addr+=program[_ip].arg2;
            _ip=addr;
            //printf("called\n");
            break;
        }
        case RET:{
            _ip=popValue();
            break;
        }
        case ADDU:{
            //printf("addu\n");
            std::vector<unsigned char> reg_codes=splitRegCode(program[_ip].arg1);
            
            unsigned short dest_code=reg_codes[0];
            unsigned short src_code=reg_codes[1];

            unsigned short dest_value=getRegister(dest_code);
            unsigned short src_value=getRegister(src_code);
            setRegister(dest_code,src_value+dest_value);
            _ip++;
            break;  
        }
        case JE:{
            std::vector<unsigned char> reg_codes=splitRegCode(program[_ip].arg1);
            unsigned short arg1=getRegister(reg_codes[0]);
            unsigned short arg2=getRegister(reg_codes[1]);
            unsigned short address_temp =program[_ip].arg2<<8;
            address_temp+=program[_ip].arg3;
            if(arg1==arg2){
                _ip=address_temp;
            }
            else{
                _ip++;
            }
            break;
        }
        case JG:{
            std::vector<unsigned char> reg_codes=splitRegCode(program[_ip].arg1);
            unsigned short arg1=getRegister(reg_codes[0]);
            unsigned short arg2=getRegister(reg_codes[1]);
            unsigned short address_temp =program[_ip].arg2<<8;
            address_temp+=program[_ip].arg3;
            if(arg1>arg2){
                _ip=address_temp;
            }
            else{
                _ip++;
            }
            break;
        }
        case JL:{
            std::vector<unsigned char> reg_codes=splitRegCode(program[_ip].arg1);
            unsigned short arg1=getRegister(reg_codes[0]);
            unsigned short arg2=getRegister(reg_codes[1]);
            unsigned short address_temp =program[_ip].arg2<<8;
            address_temp+=program[_ip].arg3;
            if(arg1<arg2){
                _ip=address_temp;
            }
            else{
                _ip++;
            }
            break;
        }
        case INT:{
            unsigned short interrupt=program[_ip].arg1<<8;
            interrupt+=program[_ip].arg2;
            _int(interrupt,_ra,_rb,_rc,_rd);
            break;
        }
        case INTR:{
            unsigned short reg_code=splitRegCode(program[_ip].arg1)[1];
            _int(getRegister(reg_code),_ra,_rb,_rc,_rd);
            break;
        }
        default:
            printf("invalid instruction\n");
            return;
            break;
        }
    }
}
void Cpu::push(char register_code){
    _sp++;
    _ram.setShort(_sp,_of,getRegister(register_code));
}
void Cpu::pop(char register_code){
    setRegister(register_code,popValue());
}
unsigned short Cpu::popValue(){
    unsigned short out=_ram.getShort(_sp,_of);
    _sp--;
    return out;
}
void Cpu::pushValue(unsigned short in){
    _sp++;
    _ram.setShort(_sp,_of,in);
}
void Cpu::setRegister(char register_code,short data){
    if(register_code>>3==1){
        //gets register last bit is zeroed out
        unsigned short mem_location=getRegister(register_code^0x80);
        _ram.setShort(mem_location,_of,data);
    }
    else{
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
std::vector<unsigned char> Cpu::splitRegCode(unsigned char reg_byte){
//1111 0000 = F0
    unsigned char lower = reg_byte&0x0F;
    //0000 1111 = 0F
    unsigned char upper = reg_byte&0xF0;
    upper=upper>>4;
    return {upper,lower};
}
void Cpu::_int(unsigned short int_code,unsigned short arg1,
            unsigned short arg2,unsigned short arg3,unsigned short arg4){
                if(int_code==0x0 && arg3==0x0){
                    init_term();
                }
                if(int_code==0x0 && arg3!=0x0){
                    printf("set char\n");
                    set_char(arg1,arg2,arg3);
                }
                printf("arg3: %i rc: %i\n",arg3,_rc);
            }
Cpu::Cpu(std::string rom_file){
    std::vector<unsigned char> rom = loadFile(rom_file);
    this->program=tokenize(rom);
    run_program();
}
Cpu::Cpu(std::vector<unsigned char> rom){
    this->program=tokenize(rom);
    run_program();
}
std::vector<unsigned short> Cpu::getRegisters(){
    std::vector<unsigned short> out={_ra,_rb,_rc,_rd,_sp,_of,_ip};
    return out;
}