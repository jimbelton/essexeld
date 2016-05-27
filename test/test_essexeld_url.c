#include "lib.h"
#include "tap.h"

int
main(void)
{
    plan_tests(2);
    is(   essexeldUrlCheck("google.com ",     10), NULL,   "Did not expect to find 'google.com' in a blacklist");
    is_eq(essexeldUrlCheck("whitehouse.com ", 14), "porn", "Expected 'whitehouse.com' to be blacklisted as 'porn'");
    return exit_status();
}
