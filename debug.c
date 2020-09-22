#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "json_type.h"
#include "debug.h"

void json_stringify(json_type_t* json){
    if(NULL==json){
        printf("undefined");
    }

    else{
        dictionary_t* aux=NULL;

        switch(json->type){
            case INT32:
                printf("%d",*(int*)json->primitive);
                break;
            case UINT32:
                printf("%u",*(int*)json->primitive);
                break;
            case ERROR:
                printf("--%d: %s--\n",json->opcode,json->message);
                break;
            case STRING:
                printf("\"%s\"",(char*)json->primitive);
                break;
            case OBJECT:

                printf("{");
                while(json){
                    aux=json->dict;
                    json_stringify(aux->key);
                    printf(":");
                    json_stringify(aux->value);
                    if(json->next) printf(",");
                    json=json->next;
                }
                printf("}");
                break;
            case ARRAY:
                printf("[");
                while(json){
                    json_stringify(json->element);
                    if(json->next) printf(",");
                    json=json->next;
                }
                printf("]");
                break;
            case TOKEN_LIST:
                token_dump(json);
        }
    }
    fflush(stdout);
}

void token_dump(token_list_t* token){

    assert(token->type==TOKEN_LIST);;
    while(token){
        if(token->token->str==NULL) printf("undefined-token ");
        else if(token->token->type==CHAR) printf("%c ",get_char_token(token->token));
        else if(token->token->type==TEXT) printf("%s ",get_string_token(token->token));
        else if(token->token->type==NUM) printf("%d ",get_integer_token(token->token));
        else if(token->token->type==UNUM) printf("%u ",get_unsigned_integer_token(token->token));
        token=token->next;
    }
    printf("\n");
    fflush(stdout);
}

char get_char_token(Token* token){
    return is_char_token(token) ? token->c : '\0';
}

char* get_string_token(Token* token){
    return is_string_token(token) ? (char*) token->str : NULL;
}

uint32_t get_unsigned_integer_token(Token* token){
    return is_integer_token(token) ? *(unsigned int*) token->str : 0;
}

int32_t get_integer_token(Token* token){
    return is_integer_token(token) ? *(int*) token->str : 0;
}

int is_unsigned_integer_token(Token* token){
    return token->type==NUM;
}
int is_integer_token(Token* token){
    return token->type==NUM;
}
int is_string_token(Token* token){
    return token->type==TEXT;
}

int is_char_token(Token* token){
    return token->type==CHAR;
}
