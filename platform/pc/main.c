#include "platform.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (!PlatformInit())
        return 1;

    while (PlatformRunFrame())
        ;

    PlatformShutdown();
    return 0;
}
