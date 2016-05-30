#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib.h"
#include "sxe-http.h"
#include "sxe-util.h"

#define DECONST(type, pointer) ((type)(uintptr_t)(pointer))

typedef struct BLOCKLIST_STRUCT {
    MD5    * md5s;
    int      fd;
    uint64_t md5Num;
} BLOCKLIST;

BLOCKLIST domainBlocklist = {MAP_FAILED, -1, 0};
BLOCKLIST urlBlocklist    = {MAP_FAILED, -1, 0};

static void
blocklistConstruct(BLOCKLIST * blocklist, const char * file) {
    uint64_t fileSize;
    SXE_STAT status;

    fileSize = sxe_stat_get_file_size(sxe_stat(&status, file));
    SXEA12(fileSize > 0, "%s domain file '%s'", sxe_stat(&status, file) == NULL ? "No such" : "Empty", file);
    SXEA13(fileSize % sizeof(MD5) == 0, "Domain file %s size %"PRIu64" is not a multiple of %u", file, fileSize,
           (unsigned)sizeof(MD5));
    blocklist->md5Num = fileSize / sizeof(MD5);
    SXEA12((blocklist->fd   = open(file, O_RDONLY)) >= 0, "Unable to open domain file %s: %s", file, strerror(errno));
    SXEA11((blocklist->md5s = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, blocklist->fd, 0)) != MAP_FAILED,
            "Unable to mempry map domain file %s", file);
}

static bool
blocklistSearch(BLOCKLIST * blocklist, const char * string, unsigned length)
{
    MD5_CTX      md5Factory;
    MD5          md5;
    MD5_Init(&md5Factory);
    MD5_Update(&md5Factory, string, length);
    MD5_Final(&md5, &md5Factory);

    return bsearch(&md5, blocklist->md5s, blocklist->md5Num, sizeof(MD5), (__compar_fn_t)md5Compare) != NULL;
}

void
essexeldUrlInit(const char * domainFile, const char * urlFile) {
    blocklistConstruct(&domainBlocklist, domainFile);

    if (urlFile != NULL) {
        blocklistConstruct(&urlBlocklist, urlFile);
    }
}

const char *
essexeldUrlCheck(char * urlString, unsigned urlLength) {
    SXE_HTTP_URL url;
    char *       hostString;
    unsigned     hostLength;
    unsigned     i;
    char *       dot;

    SXEA10(domainBlocklist.md5s != MAP_FAILED, "Domains not initialized");

    if (sxe_http_url_parse(&url, urlString, urlLength, SXE_HTTP_URL_OPTION_NOSCHEME) != SXE_RETURN_OK) {
        return NULL;
    }

    hostString = DECONST(char *, url.host);
    hostLength = url.host_length;

    for (i = 0; i < hostLength; i++) {
        hostString[i] = tolower(hostString[i]);
    }

    for (;;) {
        if (blocklistSearch(&domainBlocklist, hostString, hostLength)) {
            return "porn";
        }

        if (hostLength < 4 || (dot = memchr(hostString, '.', hostLength)) == NULL) {
            break;
        }

        hostLength -= (dot - hostString) + 1;
        hostString  = dot + 1;
    }

    if (urlBlocklist.md5s != MAP_FAILED && memchr(urlString, '/', urlLength) != NULL
     && blocklistSearch(&urlBlocklist, urlString, urlLength)) {
        return "porn";
    }

    return NULL;
}
