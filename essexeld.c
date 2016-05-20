#include "sxe-httpd.h"

int
main(int argc, char ** argv) {
    SXE_HTTPD httpd;
    SXE *     sxePtr;
    (void)argc;
    (void)argv;

    sxe_register(2, 0);
    sxe_init();
    sxe_httpd_construct(&httpd, 2, 0);
    sxePtr = sxe_httpd_listen(&httpd, "127.0.0.1", 8080);
    ev_loop(ev_default_loop(0), 0);
    sxe_close(sxePtr);
    sxe_fini();
    return 0;
}
