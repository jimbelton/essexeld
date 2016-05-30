#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

int
main(int argc, char ** argv) {
    bool     dumpFlag   = false;
    char *   filterFile = NULL;
    bool     printFlag  = false;
    FILE *   blacklist;
    char *   line;
    char     lineBuffer[1024];
    MD5 *    md5s;
    unsigned md5Max;
    unsigned md5Num;
    MD5_CTX  md5Factory;

    while (argc >= 2) {
        if (strcmp(argv[1], "-d") == 0) {
            dumpFlag = true;
        }
        else if (strcmp(argv[1], "-f") == 0) {
            filterFile = argv[2];
            argv[1] = argv[0];
            argv++;
            argc--;
        }
        else if (strcmp(argv[1], "-p") == 0) {
            printFlag = true;
        }
        else {
            break;
        }

        argv[1] = argv[0];
        argv++;
        argc--;
    }

    if (argc != 2) {
        fprintf(stderr, "usage: %s [-d] [-f <domain-md5s>] [-p] <file-name> - Output a sorted list of MD5s of the URLs in the file\n"
                        "    -d       Instead, dump an output file in hexadecimal (not yet implemented)\n"
                        "    -f file  Filter URLs, removing those whose domain names are in the file of MD5s\n"
                        "    -p       Print URLs rather than their MD5s\n",
                argv[0]);
        exit(1);
    }

    if (dumpFlag) {
        fprintf(stderr, "Dump feature is not yet implemented\n");
        exit(1);
    }

    if (filterFile) {
        essexeldUrlInit(filterFile, NULL);
    }

    if ((blacklist = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: Cant open file '%s'\n", argv[0], argv[1]);
        exit(1);
    }

    assert((md5s = malloc(sizeof(MD5))) != NULL);
    md5Max = 1;
    md5Num = 0;

    while ((line = fgets(lineBuffer, sizeof(lineBuffer), blacklist)) != NULL) {
        unsigned len;

        if (line[(len = strlen(line)) - 1] == '\n') {
            len--;
        }

        if (filterFile && essexeldUrlCheck(line, len)) {
            continue;
        }

        if (printFlag) {
            puts(line);
            continue;
        }

        if (md5Num == md5Max) {
            md5Max <<= 1;
            md5s     = realloc(md5s, md5Max * sizeof(MD5));

            if (md5s == NULL) {
                exit(1);
            }
        }

        MD5_Init(&md5Factory);
        MD5_Update(&md5Factory, line, len);
        MD5_Final(&md5s[md5Num], &md5Factory);
        md5Num++;
    }

    if (ferror(blacklist) != 0) {
        fprintf(stderr, "%s: Error %s reading file '%s'\n", argv[0], strerror(ferror(blacklist)), argv[1]);
        exit(1);
    }

    if (printFlag) {
        exit(0);
    }

    qsort(md5s, md5Num, sizeof(MD5), (__compar_fn_t)md5Compare);
    fwrite(md5s, sizeof(MD5), md5Num, stdout);
    exit(0);
}
