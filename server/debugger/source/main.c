#include <ps4.h>
#include "server.h"

int _main(void) {
    initKernel();
    initLibc();
    initPthread();
    initNetwork();
    initSysUtil();

    sceKernelSleep(1);

    // boas vindas
    sceSysUtilSendSystemNotificationWithText(222, "MEMAPI PS4 Interface loaded");
    sceSysUtilSendSystemNotificationWithText(222, "Send any payload to localhost (ps4trainer.com works)");
    
    // jailbreak
    sys_console_cmd(SYS_CONSOLE_CMD_JAILBREAK, NULL);

    // start the server
    return start_http_server();
}