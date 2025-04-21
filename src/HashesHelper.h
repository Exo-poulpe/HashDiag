#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <tomcrypt.h>


typedef struct HashHelp HashHelp;



void hhp_convert_binary_hash_to_string(unsigned char* raw_data,uint64_t size_data,char* output);

HashHelp* hhp_create_hash_helper(uint32_t hashtype);

void hhp_destroy_hash_helper(HashHelp* hhp);

char* hhp_compute_hash(HashHelp* hhp,char* txt);

char* hhp_get_hash_name(HashHelp* hhp);