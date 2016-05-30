#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib.h"
#include "sxe-http.h"
#include "sxe-util.h"

#define DECONST(type, pointer) ((type)(uintptr_t)(pointer))

MD5    * domainMd5s = MAP_FAILED;
int      domainFd;
uint64_t domainMd5Num;

void
essexeldUrlInit(const char * domainFile) {
    uint64_t domainFileSize;
    SXE_STAT status;

    domainFileSize = sxe_stat_get_file_size(sxe_stat(&status, domainFile));
    SXEA12(domainFileSize > 0, "%s domain file '%s'", sxe_stat(&status, domainFile) == NULL ? "No such" : "Empty", domainFile);
    SXEA13(domainFileSize % sizeof(MD5) == 0, "Domain file %s size %"PRIu64" is not a multiple of %u", domainFile, domainFileSize,
           (unsigned)sizeof(MD5));
    domainMd5Num = domainFileSize / sizeof(MD5);
    SXEA12((domainFd   = open(domainFile, O_RDONLY)) >= 0, "Unable to open domain file %s: %s", domainFile, strerror(errno));
    SXEA11((domainMd5s = mmap(NULL, domainFileSize, PROT_READ, MAP_SHARED, domainFd, 0)) != MAP_FAILED,
                              "Unable to mempry map domain file %s", domainFile);
}

const char *
essexeldUrlCheck(char * urlString, unsigned urlLength) {
    SXE_HTTP_URL url;
    char *       hostString;
    unsigned     hostLength;
    unsigned     i;
    char *       dot;
    MD5_CTX      md5Factory;
    MD5          hostMd5;

    SXEA10(domainMd5s != MAP_FAILED, "Domains not initialized");

    if (sxe_http_url_parse(&url, urlString, urlLength, SXE_HTTP_URL_OPTION_NOSCHEME) != SXE_RETURN_OK) {
        return NULL;
    }

    hostString = DECONST(char *, url.host);
    fprintf(stderr, "%p = %p\n", hostString, url.host);
    hostLength = url.host_length;

    for (i = 0; i < hostLength; i++) {
        hostString[i] = tolower(hostString[i]);
    }

    fputc('\n', stderr);

    for (;;) {
        MD5_Init(&md5Factory);
        MD5_Update(&md5Factory, hostString, hostLength);
        MD5_Final(&hostMd5, &md5Factory);

        if (bsearch(&hostMd5, domainMd5s, domainMd5Num, sizeof(MD5), (__compar_fn_t)md5Compare) != NULL) {
            return "porn";
        }

        if (hostLength < 4 || (dot = memchr(hostString, '.', hostLength)) == NULL) {
            break;
        }

        hostLength -= (dot - hostString) + 1;
        hostString  = dot + 1;
    }

    return NULL;
}
