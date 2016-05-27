#include "lib.h"
#include "tap.h"

int
main(void)
{
    plan_tests(5);
    is(   essexeldUrlCheck("google.com ",         10), NULL,   "Did not expect to find 'google.com' in a blacklist");
    is_eq(essexeldUrlCheck("whitehouse.com ",     14), "porn", "Expected 'whitehouse.com' to be blacklisted as 'porn'");
    is_eq(essexeldUrlCheck("www.whitehouse.com ", 18), "porn", "Expected 'www.whitehouse.com' to be blacklisted as 'porn'");
    is_eq(essexeldUrlCheck("whitehouse.com:80 ",  17), "porn", "Expected 'whitehouse.com' to be blacklisted as 'porn'");
    is_eq(essexeldUrlCheck("www.whitehouse.com/index.html ", 30), "porn",
          "Expected 'www.whitehouse.com/index,html' to be blacklisted as 'porn'");
    return exit_status();
}
