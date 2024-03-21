/*
 Title:		main.c
 Author:	Badre
 Created:	21/03/2024
 
 MD5 Code adapted for AgonLight from Zunawe (https://github.com/Zunawe/md5-c),
 forked by MilchRatchet  (https://github.com/MilchRatchet/md5-c)

 Updated code for MD5 file
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

void print_hash(uint8_t *p){
    for(unsigned int i = 0; i < 16; ++i){
        printf("%02x", p[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    uint8_t result[16];
    FILE *f = NULL;

    if(argc != 2) {
        printf("\r\nUsage: md5 [<string>|<filename>]\r\n");
        return -1;
    }

    f = fopen(argv[1], "r");
	if (f == NULL) {
		md5String(argv[1], result);
        print_hash(result);
	}
    else {
        md5File(f, result);
        print_hash(result);
        fclose(f);
    }

    /*if(argc > 1){
        for(int i = 1; i < argc; ++i){
            md5String(argv[i], result);
            print_hash(result);
        }
    }
    else{
        md5File(stdin, result);
        print_hash(result);
    }*/
}
