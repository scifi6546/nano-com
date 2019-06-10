#define TOKEN_SIZE 10
#include <stdlib.h>
#include <stdio.h>
struct string{
    char* str;
    int str_len;
};
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
    for(int i=0;i<tokens.num_tokens;i++){
        
    }
}
char * makeBin(const char* plain_text){
    int* length=calloc(sizeof(int),1);
    struct token_arr temp = tokenize(plain_text,length);
    for(int i=0;i<temp.num_tokens;i++){
        printf("token[0].comm: %s token[0].arg1: %s token[0].arg2: %s token[0].arg3: %s\n",temp.tokens[i].comm_name,temp.tokens[i].arg1,
            temp.tokens[i].arg2,temp.tokens[i].arg3); 
    }
}