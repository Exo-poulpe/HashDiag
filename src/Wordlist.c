#include "Wordlist.h"

uint64_t word_file_return_line_number(char* filepath) {
    FILE* f = fopen(filepath,"r");
    uint64_t result = 0;
    char c[128];

    if (!f)
        return 0;

    for(uint64_t i=0;fgets(c,128,f);i+=1)
        result += 1;
    
    fseek(f,0,SEEK_SET);
    return result;
    
}

uint64_t word_get_line_number(char* file_path,uint64_t cursor_pos,char* output) {
    FILE* f = fopen(file_path,"r");
    // char* res = NULL;
    char tmp[1024];
    fseek(f,cursor_pos,SEEK_SET);
    fgets(tmp, sizeof(tmp), f);
    uint64_t size_word = strlen(tmp);
    // printf("Current : %s\n",tmp);
    // res = calloc(size_word,sizeof(char));
    snprintf(output,size_word,"%s",tmp);
    cursor_pos = ftell(f);
    fclose(f);
    return cursor_pos;
}


