#include "lib.h"
#include "tap.h"

int
main(void)
{
    plan_tests(1);
    is(essexeldUrlCheck("google.com ", 10), NULL, "Did not expect to find google.com in a blacklist");
    return exit_status();
}
