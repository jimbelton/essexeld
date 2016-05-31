#include "sxe-httpd.h"
#include "lib.h"

#define DECONST(type, pointer) ((type)(uintptr_t)(pointer))

static char     pathPrefix[]  = "urlinfo/1/";
static unsigned pathPrefixLen = 10;

/* Called when an HTTP requests request line has been recieved. The expected format is:
 * GET /urlinfo/1/{hostname_and_port}/{original_path_and_query_string}
 *
 * @param request       HTTP request object
 * @param methodString  HTTP method (e.g. GET)
 * @param methodLength  Length of the method (e.g. 3)
 * @param urlString     HTTP url (e.g. http://127.0.0.1:8080/urlinfo/1/host.com/url)
 * @param urlLength     Length of the url (e.g. 44)
 * @param versionString HTTP version string
 * @param versionLength Length of the version string
 *
 * @return SXE_RETURN_OK on success, SXE_RETURN_ERROR_BAD_MESSAGE (e.g. 405 for Method Not Allowed)
 */
static SXE_RETURN
essexld_http_request(SXE_HTTPD_REQUEST * request, const char * methodString, unsigned methodLength,
                     const char * urlString, unsigned urlLength, const char * versionString, unsigned versionLength)
{
    SXE_RETURN   result = SXE_RETURN_ERROR_BAD_MESSAGE;
    SXE_HTTP_URL url;

    SXEE98I("%s(request=%p,methodString='%.*s',url='%.*s',versionString='%.*s')", __func__,
            request, methodLength, methodString, urlLength, urlString, versionLen, versionString);
    (void)versionString;
    (void)versionLength;

    if (methodLength != 3 || strncmp(methodString, "GET", 3) != 0) {
        sxe_httpd_response_simple(request, 405, "Bad request", "Invalid method. Essexeld only supports GET",
                                  HTTPD_CONNECTION_CLOSE_HEADER, HTTPD_CONNECTION_CLOSE_VALUE, NULL);
    }
    else if (sxe_http_url_parse(&url, urlString, urlLength, 0) != SXE_RETURN_OK
          || url.path_length < pathPrefixLen || strncmp(url.path, pathPrefix, pathPrefixLen) != 0) {
        fprintf(stderr, "Bad URL '%.*s'\n", url.path_length, url.path);
        sxe_httpd_response_simple(request, 400, "Bad request", "Invalid URL. Path must start with /urlinfo/1/",
                                  HTTPD_CONNECTION_CLOSE_HEADER, HTTPD_CONNECTION_CLOSE_VALUE, NULL);
    }
    else {
        request->user_data = DECONST(void *, essexeldUrlCheck(DECONST(char *, &url.path[pathPrefixLen]),
                                                              url.path_length - pathPrefixLen));
        result = SXE_RETURN_OK;
    }

    SXER81I("return result=%s", sxe_return_to_string(result));
    return result;
}

/*
 * Set the function to be called when a complete request has been recieved by an HTTPD.
 *
 * @param self        Pointer to the HTTPD
 * @param new_handler Pointer to the new handler function
 *
 * @return A pointer to the previous response function
 */

static void
essexld_http_respond(struct SXE_HTTPD_REQUEST * request)
{
    if (request->user_data == NULL) {
        sxe_httpd_response_simple(request, 404, "Not found", "URL not found", NULL);
    }
    else {
        sxe_httpd_response_simple(request, 200, "OK", request->user_data, NULL);
    }
}

int
main(int argc, char ** argv)
{
    SXE_HTTPD httpd;
    SXE *     sxePtr;
    (void)argc;
    (void)argv;

    sxe_register(3, 0);
    sxe_init();
    sxe_httpd_construct(&httpd, 3, 0);
    sxe_httpd_set_request_handler(&httpd, essexld_http_request);
    sxe_httpd_set_respond_handler(&httpd, essexld_http_respond);
    sxePtr = sxe_httpd_listen(&httpd, "127.0.0.1", 8080);
    essexeldUrlInit("data/domains", "data/urls");
    ev_loop(ev_default_loop(0), 0);
    SXEL20("I've fallen out me loop!");
    sxe_close(sxePtr);
    sxe_fini();
    return 0;
}
