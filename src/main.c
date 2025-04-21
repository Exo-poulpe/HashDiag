 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 #include <stdint.h>

#include <tomcrypt.h>

#include "Wordlist.h"
#include "HashesHelper.h"
#include "argparse.h"


static const char *const usages[] = {
    "HashDiag [options] [[--] args]",
    "HashDiag [options]",
    NULL,
};


int main(int argc,const char** argv)
{
    unsigned char* txt = "tarte";
    bool verbose = false;
    char* hash = NULL;
    char* wordlist = NULL;
    uint64_t cursor_pos = 0;
    uint32_t threads = 4;
    uint32_t algo = 0;
    HashHelp* hhp = NULL;




    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('v', "verbose", &verbose, "More verbose output",NULL,0,0),
        OPT_INTEGER('c',"core",&threads,"The number of threads used for the cracking (default : 4)",NULL,0,0),
        OPT_INTEGER('a', "algo", &algo,
                    "Hash algorithm to use:\n"
                    "\t0 : MD5     (128-bit)\n"
                    "\t1 : MD2     (128-bit)\n"
                    "\t2 : MD4     (128-bit)\n"
                    "\t3 : SHA1    (160-bit)\n"
                    "\t4 : SHA256  (256-bit)\n"
                    "\t5 : SHA512  (512-bit)",
                    NULL, 0, 0),
        OPT_STRING('t',"target",&hash,"The hash to crack, paste the hash after the options",NULL,0,0),
        OPT_STRING('w',"wordlist",&wordlist,"The wordlist to use for cracking",NULL,0,0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
    argparse_describe(&argparse,
  "\nThis program is for crack some simple hash",
  "\nExample : \n\t./HashDiag -t \"4c93008615c2d041e33ebac605d14b5b\" -w ignis.txt -c 6 -a 0");
    argc = argparse_parse(&argparse, argc, argv);

    if (!hash || !wordlist) {
        fprintf(stderr, "Error: --target and --wordlist are required\n");
        argparse_usage(&argparse);
        return -1;
    }

    hhp = hhp_create_hash_helper(algo);
    printf("Hash type to crack : %s\n",hhp_get_hash_name(hhp));
    uint64_t tmp = word_file_return_line_number(wordlist);


    omp_set_num_threads(threads);
    #pragma omp parallel
    {
        char word[1024];
        char* hashed = NULL;
        uint32_t local_pos;

        while (1) {
            
            #pragma omp critical
            {
                cursor_pos = word_get_line_number(wordlist, cursor_pos, word);
                local_pos = cursor_pos;
            }

            // Check if end of file
            if (local_pos == -1 || strlen(word) == 0) break;

            hashed = hhp_compute_hash(hhp, (unsigned char*)word);

            if (!strcmp(hash, hashed)) {
                #pragma omp critical
                {
                    printf("[+] Password found: \"%s\"\n", word);
                    free(hashed);
                    exit(0); // or set a shared flag to exit cleanly
                }
            }

            if (verbose) {
                #pragma omp critical
                {
                    printf("Thread[%d] tried: \"%s\" -> %s\n", omp_get_thread_num(), word, hashed);
                }
            }

            free(hashed);
        }
    }

    
    hhp_destroy_hash_helper(hhp);

    return 0;
}