#ifndef DEBUG_H
#define DEBUG_H 

typedef struct json_type json_type_t;
typedef struct json_type token_list_t;
typedef struct json_type json_string_t;
typedef struct _token Token;

void free_json_object_rec(json_string_t* json);

void json_stringify(json_type_t* json);

void token_dump(token_list_t* token);

char get_char_token(Token* token);

char* get_string_token(Token* token);

unsigned int get_unsigned_integer_token(Token* token);

int get_integer_token(Token* token);

int is_unsigned_integer_token(Token* token);

int is_integer_token(Token* token);

int is_char_token(Token* token);

int is_string_token(Token* token);

#endif
