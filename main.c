#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <error.h>
#include <assert.h>

#include "json_type.h"
#include "debug.h"

int main(int argc,char* argv[]){

    if(argc==1)  error(1,0,"need arguments");

    string filename,buffer;

    if(!strcmp(argv[1],"--file") || !strcmp(argv[1],"-f")){
        if(!argv[2]) error(1,0,"need filename");
        
        filename=argv[2];

        FILE* fptr=fopen(filename,"r");

        struct stat st;
        fstat(fileno(fptr),&st);

        buffer=malloc(st.st_size);
        fread(buffer,st.st_size,1,fptr);

        token_list_t* tlist = tokenize_string(buffer);
        token_list_t* aux_tlist=tlist;

        json_type_t* js = json_object_from_token(&tlist);

        json_stringify(js);

        free_json_object_rec(js);
        free_json_object_rec(aux_tlist);
        free(buffer);
        fclose(fptr);
    }
    else{
       
        fflush(stdout);
        token_list_t* tlist = tokenize_string(argv[1]);
        token_dump(tlist);
        json_type_t* js = json_object_from_token(&tlist);
 
        free_json_object_rec(js);
        free_json_object_rec(tlist);       json_stringify(js);
    }

}




