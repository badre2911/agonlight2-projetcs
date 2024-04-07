/*----------------------------------------------------------------------------
 * Title:	main.c
 * Author:	Badre
 * Created:	06/04/2024
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <mos_api.h>
//#include "strings.h"

/* File attribute bits for directory entry (FILINFO.fattrib) */
#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */

typedef struct SmallFilInfo {
    uint32_t fsize; /* File size */
    uint16_t fdate; /* Modified date */
    uint16_t ftime; /* Modified time */
    BYTE fattrib; /* File attribute */
    char* fname; /* malloc'ed */
} SmallFilInfo;

static SmallFilInfo *fnos;
static uint16_t fno_num = 0;

void list_files(const char* path/*, uint16_t from_n, uint8_t for_n*/);
static uint24_t get_num_dirents(const char* path);
static int strcasecmp(const char *s1, const char *s2);
static int cmp_filinfo(const void *a, const void *b);


int main()
{       
    list_files("/mos");
    return 0;
}

static int strcasecmp(const char *s1, const char *s2) {
    while (*s1 != '\0' && tolower((unsigned char)*s1) == tolower((unsigned char)*s2)) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}


static int cmp_filinfo(const void *a, const void *b) {
    const SmallFilInfo *fa = (const SmallFilInfo *)a;
    const SmallFilInfo *fb = (const SmallFilInfo *)b;

    if ((fa->fattrib & 0x10) == (fb->fattrib & 0x10)) {
        return strcasecmp(fa->fname, fb->fname);
    } else if (fa->fattrib & 0x10) {
        return -1;
    } else {
        return 1;
    }
}

static uint24_t get_num_dirents(const char* path)
{
    int cnt = 0;
    FRESULT fr;
    DIR dir;
    static FILINFO fno;

    fr = ffs_dopen(&dir, path);
    if (fr == FR_OK) {
        for (;;) {
            fr = ffs_dread(&dir, &fno);
            if (fr != FR_OK || fno.fname[0] == 0) {
                break; // Break on error or end of dir
            }
            cnt++;
        }
    }
    ffs_dclose(&dir);
    return cnt;
}

void list_files(const char* path) {

    FRESULT res;
    static FILINFO filinfo;
    DIR dir;
    SmallFilInfo *fno;    
   
    uint24_t num_dirents = get_num_dirents(path);
    fnos = malloc(sizeof(SmallFilInfo) * num_dirents);
    res = ffs_dopen(&dir, path);
    
    fno_num = 0;

    if (res == FR_OK) {

        while ((ffs_dread(&dir, &filinfo) == FR_OK) && filinfo.fname[0]) {

            fnos[fno_num].fsize = filinfo.fsize;
            fnos[fno_num].fdate = filinfo.fdate;
            fnos[fno_num].ftime = filinfo.ftime;
            fnos[fno_num].fattrib = filinfo.fattrib;
            fnos[fno_num].fname = malloc(strlen(filinfo.fname) + 1);
            strcpy(fnos[fno_num].fname, filinfo.fname);
            fno_num++;

        }

        ffs_dclose(&dir);
        qsort(fnos, num_dirents, sizeof(SmallFilInfo), cmp_filinfo);

        for(uint24_t i = 0; i < fno_num; i++) {
            if (fnos[i].fattrib & 0x10) printf("<%s>\r\n", fnos[i].fname);
            else printf("%s\r\n", fnos[i].fname);
        }          
    }
}
