#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "json_type.h"

void insert_list(token_list_t** t,void* token,TYPE type){
    if(*t==NULL || t==NULL){
        *t=malloc(sizeof(token_list_t));
        (*t)->token=token;
        (*t)->next=NULL;
        (*t)->type=type;
        return;
}

    token_list_t* e=*t;
    while(e->next)e=e->next;

    e->next=malloc(sizeof(token_list_t));
    e->next->token=token;
    e->next->next=NULL;
    e->type=type;
}

int length(token_list_t* t){
    int i=0;
    while(t){
    i++;
    t=t->next;
    }
    return i;
}

token_list_t* tokenize_string(char* text){
    token_list_t* t=NULL;
    Token* teta;
    char ptr[50];
    int a;
    unsigned int b;

    while(*text && *text!=EOF){
        if(*text == ' ' || *text== '\n'){
            text++;
            continue;
        }
        
        switch(*text){
            case LEFT_BRACE:
            case LEFT_BRACKET:        
            case COLON:        
            case COMMA:
            case QUOTE:
            case RIGHT_BRACE:        
            case RIGHT_BRACKET:
            case ASPAS:
                teta = malloc(sizeof(Token));
                teta->type=CHAR;
                teta->c=*text;
                text++;
                break;
            default:
                if(sscanf(text,"%d",&a)==1){
                    teta=malloc(sizeof(Token));
                    teta->type=NUM;
                    sprintf(ptr,"%d",a);
                    teta->str=malloc(sizeof(int));
                    memcpy(teta->str,&a,sizeof(int));
                    sprintf(ptr,"%d",a);
                }
                else if(sscanf(text,"%u",&b)==1){
                    teta=malloc(sizeof(Token));
                    teta->type=UNUM;
                    sprintf(ptr,"%u",a);
                    teta->str=malloc(sizeof(unsigned int));
                    memcpy(teta->str,&a,sizeof(unsigned int));
                    sprintf(ptr,"%d",a);
                }
                else {
                    sscanf(text,"%[a-zA-Z -]s%*s",ptr);
                    teta=malloc(sizeof(Token));
                    teta->type=TEXT;
                    teta->str=strdup(ptr);
                }
                text+=strlen(ptr);
        }
                insert_list(&t,teta,TOKEN_LIST);
                teta=NULL;
    }

    return t;
}






