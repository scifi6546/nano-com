# Hardware Interupts
Hardware interrupts are your way of interracting with hardware outside of the cpu.
There will be many ways to interract with the hardware. Currently only output to 
the terminal is planned.
## Terminal
To start terminal mode run with rc being set to 0:
```
int 0h
```
this switches the computer into terminal mode whre the terminal is a 40 by 40 display.
Inorder to put a charecter on screen call
```
int 0h
```
with ra bieng the x coordinate of the selected charecter rb being the y coordinate and rc the ascii charecter to put
