#include "sxe-httpd.h"
#include "lib-proto.h"

static char     urlPrefix[]  = "/urlinfo/1/";
static unsigned urlPrefixLen = 11;

/* Called when an HTTP requests request line has been recieved. The expected format is:
 * GET /urlinfo/1/{hostname_and_port}/{original_path_and_query_string}
 *
 * @param method    The HTTP method (e.g. GET)
 * @param methodLen The length of the method (e.g. 3)
 *
 * @return 0 on success or a valid HTTP response code to immediately respond with (e.g. 405 for Method Not Allowed)
 */
static SXE_RETURN
essexld_http_request(SXE_HTTPD_REQUEST * request, const char * method, unsigned methodLen,
                     const char * url, unsigned urlLen, const char * version, unsigned versionLen)
{
    SXE_RETURN result = SXE_RETURN_ERROR_BAD_MESSAGE;

    SXEE98I("%s(request=%p,method='%.*s',url='%.*s',version='%.*s')", __func__,
            request, methodLen, method, urlLen, url, versionLen, version);
    (void)version;
    (void)versionLen;

    if (methodLen != 3 || strncmp(method, "GET", 3) != 0) {
        sxe_httpd_response_simple(request, 400, "Bad request", "Invalid method. Essexeld only supports GET",
                                  HTTPD_CONNECTION_CLOSE_HEADER, HTTPD_CONNECTION_CLOSE_VALUE, NULL);
    }
    else if (urlLen < urlPrefixLen || strncmp(url, urlPrefix, urlPrefixLen) != 0) {
        sxe_httpd_response_simple(request, 400, "Bad request", "Invalid URL. Must start with /urlinfo/1/",
                                  HTTPD_CONNECTION_CLOSE_HEADER, HTTPD_CONNECTION_CLOSE_VALUE, NULL);
    }
    else {
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
    char * response;

    if ((response = essexeldCheckUrl(&request->url[urlPrefixLen], request->url_length - urlPrefixLen)) == NULL) {
        sxe_httpd_response_simple(request, 404, "Not found", "URL not found", NULL);
    }
    else {
        sxe_httpd_response_simple(request, 200, "OK", response, NULL);
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
    ev_loop(ev_default_loop(0), 0);
    SXEL20("I've fallen out me loop!");
    sxe_close(sxePtr);
    sxe_fini();
    return 0;
}
