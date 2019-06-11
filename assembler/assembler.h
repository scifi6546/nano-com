#ifndef assembler_h
#define assebler_h
struct string{
    char* str;
    int str_len;
};
//makes binary from plain test assembly program
struct string makeBin(const char* plain_text);

#endif