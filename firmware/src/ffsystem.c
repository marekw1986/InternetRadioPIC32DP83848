#include "ff.h"      // defines BYTE, FF_SYNC_t, etc.
#include "ffconf.h"

#if FF_FS_REENTRANT  // Only compile this if reentrancy is enabled

#include "FreeRTOS.h"
#include "semphr.h"

int ff_cre_syncobj(BYTE vol, FF_SYNC_t *sobj) {
    *sobj = xSemaphoreCreateMutex();
    return (*sobj != NULL);
}

int ff_del_syncobj(FF_SYNC_t sobj) {
    vSemaphoreDelete((SemaphoreHandle_t)sobj);
    return 1;
}

int ff_req_grant(FF_SYNC_t sobj) {
    return (xSemaphoreTake((SemaphoreHandle_t)sobj, FF_FS_TIMEOUT / portTICK_PERIOD_MS) == pdTRUE);
}

void ff_rel_grant(FF_SYNC_t sobj) {
    xSemaphoreGive((SemaphoreHandle_t)sobj);
}

#endif
