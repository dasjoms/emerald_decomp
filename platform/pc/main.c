#include "platform.h"
#include "main.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (!PlatformInit())
        return 1;

    while (GameLoop() && PlatformRunFrame())
        ;

    PlatformShutdown();
    return 0;
}
