#include <stdio.h>
#include <string.h>

#include "lib.h"
#include "sxe-http.h"
//#include "sxe-util.h"
//
//struct DOMAIN {
//    MD5  md5;
//    char value[16];
//} * domains;
//
//essexeldUrlInit(const * domainFile) {
//    SXE_STAT status;
//    off_t size = sxe_stat_get_file_size(sxe_stat(&status, domainFile));
//    SXEA12(size > 0, " domain file '%s'", sxe_stat(&status, domainFile) == NULL ? "No such" : "Empty", domainFile);
//}

const char *
essexeldUrlCheck(const char * urlString, unsigned urlLength) {
    SXE_HTTP_URL url;
    const char * hostString;
    unsigned     hostLength;
    const char * dot;

    if (sxe_http_url_parse(&url, urlString, urlLength, SXE_HTTP_URL_OPTION_NOSCHEME) != SXE_RETURN_OK) {
        return NULL;
    }

    hostString = url.host;
    hostLength = url.host_length;

    for (;;) {
        if (strncmp("whitehouse.com", hostString, hostLength) == 0) {
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
