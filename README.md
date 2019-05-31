## nano-com
# What is this?
This is a project to build a fictional 16 bit console. It will have a limited amount of memory (lets say 1MB), 
a simple instruction set to make it easy to program, an easy way to share and load games and a screen keyboard and mouse interface.

# Specs of computer
The computer will have 1 MB of addressible ram. An interface for a cartridge and non volitile storage and a cpu with a limited speed.

# Instruction Set
Details still to be determined but I am thinking of having a very simple instruction set. 

# Opcode Format:
A full instruction consists of (except for jmp) two bytes. The first specifies the instruction used and the second byte are 
the arguments of the instruction

# Registers
There are 4 general purpose registars ra rb rc and rd. These registers can be used for anything. There are two special purpose
registars, sp the stack pointer and of, the offset register.

| Register                 | Opcode (in binary) |
| ------------------------ | ------------------ |
| ra (general purpose)     |   000              |
| rb (general purpose)     |   001              |
| rc (general purpose      |   010              |
| rd (general purpose)     |   011              |
| sp (stack pointer)       |   100              |
| of (offset register)     |   101              |
| unused                   |   110              |
| unused                   |   111              |

# Instruction listing

| Instruction          | Opcode (in binary)    | Description                | Arguments                              |
| -------------------- | --------------------- | -------------------------- | -------------------------------------- |
| term                 |  0000 0000            | Halts the machine          | None                                   |
| move                 |  0000 0001            | moves data                 | 1 byte first 4 bits dest last 4 source |
| jump                 |  0000 0010            | jumps to memory address    | 2 bytes address to jump too            |
| add                  |  0000 0011            | adds two registers together| 1 byte first 4 bits dest last 4 source |
| sub                  |  0000 0100            | subtracts to registers     | 1 byte first 4 bits dest last 4 source |
