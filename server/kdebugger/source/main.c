#include <ksdk.h>
#include "hooks.h"

int _main(void) {
    init_ksdk();

    printf("[PS4MEMAPI] kernel base 0x%llX\n", get_kbase());

    if(install_hooks()) {
        printf("[PS4MEMAPI] failed to install hooks\n");
        return 1;
    }

    return 0;
}
