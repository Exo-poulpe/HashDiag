#include "HashesHelper.h"


typedef struct HashHelp {
    struct ltc_hash_descriptor* hash_desc;
    char* name;
    uint32_t hashsize;
} HashHelp;


void hhp_convert_binary_hash_to_string(unsigned char* raw_data,uint64_t size_data,char* output) {
    for (uint64_t i = 0; i < size_data; i+=1) {
        sprintf(&output[i * 2], "%02x", raw_data[i]);
    }
    output[size_data * 2] = '\0';
}


HashHelp* hhp_create_hash_helper(uint32_t hashtype) {
    struct HashHelp* hhp = calloc(1,sizeof(HashHelp));
    hhp->name = calloc(128,sizeof(char));


    switch (hashtype)
    {

    case 0:
        hhp->hash_desc = &md5_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"MD5");
        break;
    case 1:
        hhp->hash_desc = &md2_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"MD2");
        break;
    case 2:
        hhp->hash_desc = &md4_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"MD4");
        break;
    case 3:
        hhp->hash_desc = &sha1_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"SHA1");
        break;
    case 4:
        hhp->hash_desc = &sha256_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"SHA256");
        break;
    case 5:
        hhp->hash_desc = &sha512_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"SHA512");
        break;
    
    default:
        hhp->hash_desc = &md5_desc;
        hhp->hashsize = hhp->hash_desc->hashsize;
        sprintf(hhp->name,"MD5");
        break;
    }

    
    return hhp;

}


char* hhp_get_hash_name(HashHelp* hhp){
    char* res = calloc(128,sizeof(char));
    sprintf(res,"%s",hhp->name);
    return res;
}


void hhp_destroy_hash_helper(HashHelp* hhp) {
    free(hhp->name);
    free(hhp);
}


char* hhp_compute_hash(HashHelp* hhp,char* txt){

    
	hash_state md;
	hhp->hash_desc->init(&md);

    unsigned long hashsize = hhp->hash_desc->hashsize;
    unsigned char res[hashsize];
    char* hashed = calloc(hashsize * 2 + 1, sizeof(char));

	hhp->hash_desc->process(&md, txt, strlen(txt));   
	hhp->hash_desc->done(&md, res);


    hhp_convert_binary_hash_to_string(res,hhp->hash_desc->hashsize,hashed);


    return hashed;

}