#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>

typedef void* T;
typedef char* string;


typedef struct json_type json_type_t;
typedef struct json_type token_list_t;
typedef struct json_type json_string_t;
typedef struct json_type json_dictionary_t;
typedef struct _dict dictionary_t;

typedef enum{
    ARRAY,
    TOKEN_LIST,
    OBJECT,
    UINT32,
    INT32,
    STRING,
    ERROR
}TYPE;

typedef enum{
    INVALID_TOKEN,
    MISMATCH_QUOTES,
    MISSING_BRACE,
    MISSING_BRACKET,
    MISSING_COLON,
    MISSING_ASPAS
}ERROR_TYPE;

typedef enum{
    UNUM,
    NUM,
    CHAR,
    TEXT,
}TYPE_TEXT;

typedef enum{
    LEFT_BRACE='[',
    RIGHT_BRACE=']',
    LEFT_BRACKET='{',
    RIGHT_BRACKET='}',
    COMMA=',',
    COLON=':',
    ASPAS='\"',
    QUOTE='\''
}SPECIAL_CHARACTER;

typedef struct _token{
    TYPE_TEXT type;
    union{
        char c;
        T str;
    };
}Token;

struct _dict{
    json_string_t* key;
    json_type_t* value;
};


struct json_type{
    TYPE type;
    union{
        T primitive;

        struct{
            union{
                json_type_t* element;
                Token* token;
                dictionary_t* dict;
            };

            json_type_t* next;
        };

        struct{
            string message;
            ERROR_TYPE opcode;
        };

        long long align;
    };
} __attribute__((packed));

json_type_t* json_object_from_token(token_list_t** t);
json_type_t* new_json_string(token_list_t** tlist);
json_type_t* new_json_array(token_list_t** tlist);
json_type_t* new_json_object(token_list_t** tlist);

token_list_t* tokenize_string(char* text);
void insert_list(token_list_t** t,void* token,TYPE type);

void json_stringify(json_type_t* json);
void token_dump(token_list_t* token);






#endif
