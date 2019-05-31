### nano-com
## What is this?
This is a project to build a fictional 16 bit console. It will have a limited amount of memory (lets say 1MB), 
a simple instruction set to make it easy to program, an easy way to share and load games and a screen keyboard and mouse interface.

## Specs of computer
The computer will have 1 MB of addressable ram. An interface for a cartridge and non volatile storage and a cpu with a limited speed.

## Instruction Set
Details still to be determined but I am thinking of having a very simple instruction set. 

## Opcode Format:
A full instruction consists of (except for jmp) two bytes. The first specifies the instruction used and the second byte are 
the arguments of the instruction

## Registers
There are 4 general purpose registers ra rb rc and rd. These registers can be used for anything. There are two special purpose
registers, sp the stack pointer and of, the offset register.

| Register                 | Opcode (in binary) |
| ------------------------ | ------------------ |
| ra (general purpose)     |   000              |
| rb (general purpose)     |   001              |
| rc (general purpose      |   010              |
| rd (general purpose)     |   011              |
| sp (stack pointer)       |   100              |
| of (offset register)     |   101              |
| ip (instruction pointer) |   110              |
| unused                   |   111              |

## Instruction listing

| Instruction          | Opcode (in binary)    | Description                      | Arguments                                |
| -------------------- | --------------------- | -------------------------------- | ---------------------------------------- |
| term                 |  0000 0000            | Halts the machine                | None                                     |
| push                 |  0000 0001            | pushs address onto stack         | 1 byte last three bytes contain register |
| pop                  |  0000 0010            | pops address of of stack         | 1 byte last three bytes contain register |`
| move                 |  0000 0011            | moves data                       | 1 byte first 4 bits dest last 4 source   |
| movc                 |  0000 0100            | moves constant into register     | 1st byte register 2 bytes constant       |
| jump                 |  0000 0101            | jumps to memory address          | 2 bytes address to jump too              |
| call                 |  0000 0110            | calls address                    | 2 butes address to jump too              |
| ret                  |  0000 0111            | pops address of of stack into ip | None                                     |
| addu                 |  0000 1000            | adds two registers together      | 1 byte first 4 bits dest last 4 source   |
| adds                 |  0000 1001            | adds signed                      | 1 byte first 4 bits dest last 4 source   |
| sub                  |  0000 1010            | subtracts to registers           | 1 byte first 4 bits dest last 4 source   |

## Instructions Detail

# term
This just terminates the machine.
```
Opcode: 
0000 0000  0000 0000  0000 0000
````

# push
Pushes reg onto stack. sp is incremented by two bytes (by two)
```
opcode:
0000 0001   0000 [reg]  0000 0000
```

# pop
Pops data from stack into register. sp is decremented by two
```
opcode:
0000 0010  0000 [reg] 0000 0000

```

# move
Moves data from one register into another register
```
opcode:
0000 0011 [dest reg][src reg]   0000 0000   0000 0000
```

# movc
Moves constant value into a register
```
opcode:
0000 0100 [dest reg]  [constant value 2 bytes]
```

# jump
jumps to specified address
```
opcode:
0000 0101 [2 bytes dest address]
```

# call
jumps to a specified address and then pushes next instruction on stack. It pushes the current value of ip+=1
```
opcode:
0000 0110 [2 bytes dest address]
```

# ret
jumps to instruction pushed off of stack. It pushes a value off of the stack into ip and goes to it.
```
opcode:
0000 0111  0000 0000   0000 0000  0000 0000
```
# addu
adds two unsigned registers together. result is stored in dest
```
opcode:
0000 1000 [dest] [src]  0000 0000  0000 0000
```

# adds
adds two signed registers together. result is stored in dest
```
opcode:
0000 1001 [dest] [src]   0000 0000  0000 0000
```
