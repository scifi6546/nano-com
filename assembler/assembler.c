#define TOKEN_SIZE 10
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "assembler.h"
struct string cpyword(const char* text_file){
    struct string out;
    out.str = calloc(sizeof(char),TOKEN_SIZE);
    out.str_len=0;
    for(int i=0;i<TOKEN_SIZE;i++){
        char temp = text_file[i];
        if(temp=='\0' || temp==' ' || temp=='\n'){
            printf("i: %i\n",i);
            out.str_len=i-1;
            return out;
        }else{
            out.str[i]=temp;
        }
    }
    return out;
}
//returns a to the power of b
int power(int a, int b){
    int out=a;
    for(int i=0;i<b-1;b++){
        out*=a;
    }
    return out;
}
enum DEC_SYSTEM{BIN,OCTAL,DEC,HEX};
//turns string into a short
short StringtoShort(const char *in){
    printf("char in: %s\n",in);
    char *digits=malloc(0);
    int digits_len=0;
    int dec_or_hex=0;
    for(int i=0;;i++){
        if(in[i]=='\0')
            break;
        if(in[i]>='0'&&in[i]<='9'){
            printf("num found\n");
            digits=realloc(digits,digits_len+1);
            digits_len++;
            digits[i]=in[i]-'0';
        }
        if(in[i]=='O'){
            dec_or_hex=OCTAL;
            break;
        }
        if(in[i]=='D'){
            dec_or_hex=DEC;
            break;
        }
        if(in[i]=='h'){
            dec_or_hex=HEX;
            break;
        }
    }
    short out=0;
    int digit=0;
    int raise_to=0x0A;
    if(dec_or_hex==HEX)
        raise_to=0x10;
    printf("out (before) %i\n",out);
    printf("digits_len: %i\n",digits_len);
    for(int i=digits_len-1;i>=0;i--){
        digit++;
        printf("raise_to: %i digit: %i\n",raise_to,digit);
        printf("power: %i\n",power(raise_to,digit));
        out+=digits[i]*power(raise_to,digit);
        printf("out: %i\n",out);
    }
    printf("out final: %i\n",out);
    return out;
}
struct token{
    char *comm_name;
    char *arg1;
    char *arg2;
    char *arg3;
};
struct token_arr{
    struct token *tokens;
    int num_tokens;
};
//adds char to end of string reallocs dynamically
struct string addChar(struct string in,char char_in){
    in.str=realloc(in.str,in.str_len+1);
    in.str[in.str_len]=char_in;
    in.str_len++;
    return in;
}
//gets opcode for single register from string
char getRegOpcodeS(const char* reg){
    char mem_access=0x0;
    printf("reg: %p\n",reg);
    if(reg[0]=='\0'){
        printf("error!!!");
        return 0x0;
    }
    if(reg[0]=='['&&reg[1]!='\0'){
        if(reg[2]==']'){
            mem_access=0xF;
        }
    }
    if(strncmp(reg,"ra",TOKEN_SIZE)==0)
        return mem_access+0x0;
    if(strncmp(reg,"rb",TOKEN_SIZE)==0)
        return mem_access+0x1;
    if(strncmp(reg,"rc",TOKEN_SIZE)==0)
        return mem_access+0x2;
    if(strncmp(reg,"rd",TOKEN_SIZE)==0)
        return mem_access+0x3;
    if(strncmp(reg,"sp",TOKEN_SIZE)==0)
        return mem_access+0x4;
    if(strncmp(reg,"of",TOKEN_SIZE)==0)
        return mem_access+0x5;
    if(strncmp(reg,"ip",TOKEN_SIZE)==0)
        return mem_access+0x6;
}
char getRegOpcodeM(const char* reg1, const char *reg2){
    return (getRegOpcodeS(reg1)<<4)+getRegOpcodeS(reg2);
}
enum TOKEN_PART{COMM_NAME=0x0,ARG1=0x1,ARG2=0x2,ARG3=0x3};
//makes easy to parse tokens out of plain text
struct token_arr tokenize(const char* plain_text,int * token_length_out){
    token_length_out=calloc(sizeof(int),1);
    struct token_arr out;
    out.num_tokens=100;
    out.tokens = calloc(out.num_tokens,sizeof(struct token));
    int token_index=0;//index in token array
    int part_of_token=0;//tells which part of token is currently being worked on
    for(int i=0;;i++){
        if(plain_text[i]=='\0'){
            return out;
        }
        else if(plain_text[i]=='\n'){
            part_of_token=COMM_NAME;
            token_index++;
            printf("end of line reached\n");
            if(token_index==out.num_tokens){
                out.tokens=realloc(out.tokens,out.num_tokens+100);
                out.num_tokens+=100;
            }
        }
        else if(plain_text[i]==' '){
            printf("part of token increased!\n");
            part_of_token++;
        }
        else{
            int *word_len = calloc(sizeof(int),1);
            struct string temp = cpyword(plain_text+i);
            printf("str_len: %i\n",temp.str_len);
            printf("part of token: %i\n",part_of_token);
            if(part_of_token==0){
                out.tokens[token_index].comm_name=temp.str;
                //part_of_token++;
                i+=temp.str_len;
            }else if(part_of_token==1){
                out.tokens[token_index].arg1=temp.str;
                //part_of_token++;
                i+=temp.str_len;
            }else if(part_of_token==2){
                out.tokens[token_index].arg2=temp.str;
                //part_of_token++;
                i+=temp.str_len;
            }else if(part_of_token==3){
                out.tokens[token_index].arg3=temp.str;
                //part_of_token++;
                i+=temp.str_len;
            }else if(part_of_token>=3){
                part_of_token=0;
                printf("invalid token reset\n");
            }
            printf("part of token: %i\n",part_of_token);
        }
    }
    return out;
}
struct string toOpcode(const struct token_arr tokens){
    struct string out;
    out.str=calloc(1,0);
    out.str_len=0;
    for(int i=0;i<tokens.num_tokens;i++){
        if(tokens.tokens[i].comm_name==0x0){
            return out;
        }
        if(strncmp(tokens.tokens[i].comm_name,"term",TOKEN_SIZE)==0){
            out=addChar(out,0x0);
            out=addChar(out,0x0);
            out=addChar(out,0x0);
            out=addChar(out,0x0);
            printf("term found\n");
        }

        if(strncmp(tokens.tokens[i].comm_name,"push",TOKEN_SIZE)==0){
            printf("push found\n");
            out=addChar(out,0x1);
            out=addChar(out,getRegOpcodeS(tokens.tokens[i].arg1));
            out=addChar(out,0x0);
            out=addChar(out,0x0);
        }
        if(strncmp(tokens.tokens[i].comm_name,"pop",TOKEN_SIZE)==0){
            out=addChar(out,0x2);
            out=addChar(out,getRegOpcodeS(tokens.tokens[i].arg1));
            out=addChar(out,0x0);
            out=addChar(out,0x0);
        }
        if(strncmp(tokens.tokens[i].comm_name,"move",TOKEN_SIZE)==0){
            printf("comm_name: %s\n",tokens.tokens[i].comm_name);
            printf("strncmp: %i\n",strncmp(tokens.tokens[i].comm_name,"move",TOKEN_SIZE));
            printf("move called\n");
            out=addChar(out,0x3);
            out=addChar(out,getRegOpcodeM(tokens.tokens[i].arg1,tokens.tokens[i].arg2));
            out=addChar(out,0x0);
            out=addChar(out,0x0);
        }
        if(strncmp(tokens.tokens[i].comm_name,"movc",TOKEN_SIZE)==0){
            out=addChar(out,0x4);
            out=addChar(out,getRegOpcodeS(tokens.tokens[i].arg1));
            short num=StringtoShort(tokens.tokens[i].arg2);
            out=addChar(out,num>>8);
            num=num&0x0f;
            out=addChar(out,num);
        }
        if(strncmp(tokens.tokens[i].comm_name,"jump",TOKEN_SIZE)==0){
            out=addChar(out,0x5);
            short num=StringtoShort(tokens.tokens[i].arg1);
            out=addChar(out,num>>8);
            num=num&0x0f;
            out=addChar(out,num);
            out=addChar(out,0x0);
        }
        if(strncmp(tokens.tokens[i].comm_name,"call",TOKEN_SIZE)==0){
            out=addChar(out,0x6);
            short num=StringtoShort(tokens.tokens[i].arg1);
            out=addChar(out,num>>8);
            num=num&0x0f;
            out=addChar(out,num);
            out=addChar(out,0x0);
        }
        if(strncmp(tokens.tokens[i].comm_name,"ret",TOKEN_SIZE)==0){
            out=addChar(out,0x7);
            out=addChar(out,0x0);
            out=addChar(out,0x0);
            out=addChar(out,0x0);
        }

    }
}
struct string makeBin(const char* plain_text){
    int* length=calloc(sizeof(int),1);
    struct token_arr temp = tokenize(plain_text,length);
    struct string out = toOpcode(temp);
    /* 
    for(int i=0;i<temp.num_tokens;i++){
        printf("token[0].comm: %s token[0].arg1: %s token[0].arg2: %s token[0].arg3: %s\n",temp.tokens[i].comm_name,temp.tokens[i].arg1,
            temp.tokens[i].arg2,temp.tokens[i].arg3); 
    }*/
    //printf("opcode: %s\n",out.str);
    return out;
}