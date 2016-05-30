#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"
#include "tap.h"

static void
isCheck(const char * url, const char * category)
{
    char urlBuffer[256];

    strcpy(urlBuffer, url);

    if (category == NULL) {
        is(essexeldUrlCheck(urlBuffer, strlen(urlBuffer) - 1), NULL, "Did not expect to find '%s' in the blacklist", urlBuffer);
    }
    else {
        is_eq(essexeldUrlCheck(urlBuffer, strlen(urlBuffer) - 1), category,
              "Expected '%s' to be blacklisted as '%s'", urlBuffer, category);
    }
}

int
main(int argc, char ** argv)
{
    char     domainPath[PATH_MAX];
    char *   lastSlash;
    unsigned prefixLen;
    char     urlPath[PATH_MAX];

    (void)argc;
    plan_tests(12);
    ok(realpath(argv[0], domainPath)          != NULL, "Unable to compute the absolute path from '%s'",       argv[0]);
    ok((lastSlash = strrchr(domainPath, '/')) != NULL, "Unable to find a trailing '/' in absolute path '%s'", domainPath);
    *lastSlash = '\0';
    ok((lastSlash = strrchr(domainPath, '/')) != NULL, "Unable to find a another '/' in absolute path '%s'",  domainPath);
    *lastSlash = '\0';
    ok((lastSlash = strrchr(domainPath, '/')) != NULL, "Unable to find a third '/' in absolute path '%s'",  domainPath);
    prefixLen = lastSlash - domainPath + 1;
    strncpy(lastSlash + 1, "data/domains", PATH_MAX - prefixLen);
    domainPath[PATH_MAX - 1] = '\0';
    strncpy(urlPath, domainPath, prefixLen);
    strncpy(&urlPath[prefixLen], "data/urls", PATH_MAX - prefixLen);
    urlPath[PATH_MAX - 1] = '\0';
    essexeldUrlInit(domainPath, urlPath);

    isCheck("google.com ",                              NULL);
    isCheck("ThePornDude.com\r",                        "porn");
    isCheck("whitehouse.com ",                          "porn");
    isCheck("www.whitehouse.com ",                      "porn");
    isCheck("whitehouse.com:80 ",                       "porn");
    isCheck("www.whitehouse.com/index.html ",           "porn");
    isCheck("abcnews.go.com",                           NULL);
    isCheck("abcnews.go.com/health/video/strange-sex ", "porn");
    return exit_status();
}
