#include <stdio.h>

#include "lib.h"
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
essexeldUrlCheck(const char * url, unsigned urlLen) {
    (void)url;
    (void)urlLen;
    return NULL;
}
