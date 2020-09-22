#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "json_type.h"
#include "debug.h"



void* move(void** addr){
    void* var = *addr;
    *addr=NULL;
    return var;
}

void free_json_object_rec(json_type_t* json){
    json_type_t* baseptr;

    switch(json->type){
        case UINT32:
        case INT32:
        case STRING:
            free(json->primitive);
            free(json);
            break;
        case ERROR:
            free(json->message);
            free(json);
            break;
        case TOKEN_LIST:
            while(json){
                if(json->token->type==TEXT && json->token->str) free(json->token->str);
                free(json->token);

                baseptr=json;

                json=json->next;
                free(baseptr);
            }
            break;
        case ARRAY:
            while(json){
                free_json_object_rec(json->element);

                baseptr=json;

                json=json->next;
                free(baseptr);

            }
            break;
        case OBJECT:
            while(json){
                free_json_object_rec(json->dict->key);
                free_json_object_rec(json->dict->value);
                free(json->dict);

                baseptr=json;

                json=json->next;
                free(baseptr);
            }
    }
}

dictionary_t* new_dictionary(json_string_t* key,json_type_t* value){
    dictionary_t* d = malloc(sizeof(dictionary_t));
    *d = (dictionary_t) {.key=key,.value=value}; 
    return d;
}

json_type_t* shift(token_list_t** tlist){
    return (*tlist) = (*tlist)->next;
}

token_list_t* last_list(token_list_t* tlist){
    while(tlist->next) tlist=tlist->next;
    return tlist;
}

json_type_t* new_json_type(TYPE type){
    json_type_t* js = malloc(sizeof(json_type_t));
    js->type=type;
    js->align=0;
    return js;
}

json_type_t* json_object_error(char* message,ERROR_TYPE opcode){
    json_type_t* js=new_json_type(ERROR);
    js->message=message;
    js->opcode=opcode;
    return js;
}

json_type_t* json_object_from_token(token_list_t** t){
    json_type_t* js=NULL;

    if((*t)->token->type==CHAR){
        char c = (*t)->token->c;

        switch(c){
            case RIGHT_BRACE: 
            case RIGHT_BRACKET:
            case COMMA:
            case COLON:
                js=json_object_error("invalid colon",INVALID_TOKEN);
                break;
            case LEFT_BRACE:
               js=new_json_array(t); 
                break;
             case LEFT_BRACKET:
               js=new_json_object(t); 
                break;
            case ASPAS:
            case QUOTE:
               js=new_json_string(t); 
            default:break;
        }
    }
    else{
        switch((*t)->token->type){
            case NUM:
                    js=new_json_type(INT32);
                    js->primitive=move(&(*t)->token->str);
                    shift(t);
                    break;

            case UNUM:
                    js=new_json_type(UINT32);
                    js->primitive=move(&(*t)->token->str);
                    shift(t);
                    break;
            default:
                    fprintf(stderr,"wtfff");
        }
    }

    return js;
}


json_type_t* new_json_string(token_list_t** tlist){
    json_type_t* js=NULL;

    Token* left=(*tlist)->token;

        shift(tlist);

    Token* string=(*tlist)->token;

        shift(tlist);

    Token* right=(*tlist)->token;

        shift(tlist);


    char quoting_char_left = left->c,quoting_char_right=right->c;

    if(quoting_char_left != ASPAS && quoting_char_left!=QUOTE){
        js=json_object_error("wrong quote",INVALID_TOKEN);
        return js;
    }
    
    if(quoting_char_left !=quoting_char_right){
         js=json_object_error("quotes do not match",MISMATCH_QUOTES);
         return js;
    }
    
    js=new_json_type(STRING);
    js->primitive=move(&string->str);
    
    return js;
}

json_type_t* new_json_array(token_list_t** tlist){
    Token* left=(*tlist)->token;

    json_type_t* js=NULL;

    if(left->type!=CHAR || left->c!= LEFT_BRACE){
        js=json_object_error("missing left paren",MISSING_BRACE);
        return js;
    }
    shift(tlist);

    while(!((*tlist)->token->c==RIGHT_BRACE)){
        if((*tlist)->token->type==CHAR && (*tlist)->token->c==COMMA){
            js=json_object_error("what is this comma doing here",INVALID_TOKEN);
            return js;
        }

        insert_list(&js,json_object_from_token(tlist),ARRAY);
        if((*tlist)->token->c!=RIGHT_BRACE) shift(tlist);
    }
   
    return js;
}

json_type_t* new_json_object(token_list_t** tlist){
    Token* left=(*tlist)->token;

    json_type_t* js=NULL;

    if(left->type!=CHAR || left->c!= LEFT_BRACKET){
        js=json_object_error("missing left brace",INVALID_TOKEN);
        return js;
    }

        shift(tlist);

    while(!((*tlist)->token->type==CHAR) || !((*tlist)->token->c==RIGHT_BRACKET)){
        if((*tlist)->token->type==CHAR && (*tlist)->token->c==COMMA){
            js = new_json_type(ERROR);
            js->message=strdup("wrong brackets");
            js->opcode=INVALID_TOKEN;
            return js;
        }

        json_string_t* str =json_object_from_token(tlist);

        if(is_char_token((*tlist)->token) && get_char_token((*tlist)->token)!=COLON){
            js=json_object_error("missing colon",MISSING_COLON);
            return js;
        }

        shift(tlist);
        

        json_type_t* value =json_object_from_token(tlist);

        insert_list(&js,new_dictionary(str,value),OBJECT);

        if((is_char_token((*tlist)->token)) && (*tlist)->token->c!=RIGHT_BRACKET) shift(tlist);
    }

    shift(tlist);
   
    return js;
}





