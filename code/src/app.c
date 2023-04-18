/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include "system/debug/sys_debug.h"
//#include "ff.h"
#include "app.h"
#include "system_config.h"
#include "system_definitions.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usb/usb_host.h"
#include "usb/usb_host_msd.h"
//#include "driver/usb/usbhs/src/usbhs_registers.h"
#include "vs1003/vs1003.h"
#include "io/buttons.h"
#include "io/rotary.h"
#include "system/fs/sys_fs.h"
#include "tcpip/tcpip.h"
#include "tcpip/http_net.h"
#include "http_net_print.h"
#include "lcd/hd44780.h"
#include "lcd/ui.h"
#include "lcd/i2c.h"
#include "common.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
button_t next_btn;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

void usb_write (void);
void next_func (void);

void usb_write (void) { 
    SYS_FS_HANDLE file;
    int32_t fileSize;
    int32_t res;

    file = SYS_FS_FileOpen("/mnt/myDrive1/file.txt", SYS_FS_FILE_OPEN_APPEND);   //(&file, "0:/test.txt", (FA_OPEN_ALWAYS | FA_WRITE));
    if (file == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "SYS_FS_FileOpen error\r\n");
        return;
    }
    fileSize = SYS_FS_FileSize(file);
    if (fileSize == 0) {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "To jest pierwsza linia\r\n", &file);
    }
    else {
        res = SYS_FS_FileSeek(file, fileSize, SYS_FS_SEEK_SET);
        if ( (res == -1) || (res != fileSize) ) {
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "f_lseek error code: %i\r\n", res); 
            SYS_FS_FileClose(file);
            return;
        }
    }
    SYS_FS_FilePrintf(file, "Test\r\n");
    SYS_FS_FileClose(file); 
}

void next_func (void) {
    SYS_CONSOLE_PRINT("NEXT button pressed!\r\n");
    VS1003_stop();
    VS1003_play_next_http_stream_from_list();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    appData.sdCardConnected = false;
    appData.usbDriveConnected = false;
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


// *****************************************************************************
/* Function:
    USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler 
    (
        USB_HOST_EVENT event, 
        void * eventData, 
        uintptr_t context
    )
  Summary:
    This function processes USB Host Layer Events.
  Description:
    This function processes USB Host Layer Events.
  Remarks:
    None.
*/    

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
    }

    return(USB_HOST_EVENT_RESPONSE_NONE);
}

// *****************************************************************************
/* Function:
    void APP_SYSFSEventHandler
    (
        SYS_FS_EVENT event, 
        void *mountName, 
        uintptr_t context
    )
  Summary:
    This function processes File System Events.
  Description:
    This function processes File System Events.
  Remarks:
    None.
*/    

void APP_SYSFSEventHandler(SYS_FS_EVENT event, void *mountName, uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:

            if(0 == strcmp((const char *)mountName,"/mnt/myDrive0"))
            {
                appData.sdCardConnected  = true;
            }

            else if(0 == strcmp((const char *)mountName,"/mnt/myDrive1"))
            {
                appData.usbDriveConnected = true;
            }
            break;

        case SYS_FS_EVENT_UNMOUNT:

            if(0 == strcmp((const char *)mountName,"/mnt/myDrive0"))
            {
                appData.sdCardConnected  = false;
            }

            else if(0 == strcmp((const char *)mountName,"/mnt/myDrive1"))
            {
                appData.usbDriveConnected = false;
            }

            break;

        default:
            break;
    }
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    static uint32_t timer = 0;
    static uint8_t lcd_light = 0;
    SYS_STATUS tcpipStat;
    SYS_STATUS usbHostStat;
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            SYS_CONSOLE_PRINT("Initializing user app\r\n");
            
            rotary_init();
            button_init(&next_btn, &PORTG, _PORTG_RG13_MASK, &next_func, NULL);
            
            //i2c_init();
            lcd_init();
            lcd_cls();
            lcd_ui_draw_interface();          
            
            VS1003_begin();
            VS1003_setVolume(100);
            vTaskDelay(100);
            SYS_FS_EventHandlerSet((void *)APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);            
            USB_HOST_BusEnable(0);            
            appData.state = APP_STATE_USB_HOST_WAIT_INIT;
        }
        
        case APP_STATE_USB_HOST_WAIT_INIT:
        {
            usbHostStat = USB_HOST_Status(sysObj.usbHostObject0);
            if(usbHostStat < 0) {
                // some error occurred
                SYS_CONSOLE_PRINT("USB HOST initialization failed!\r\n");
            }
            else if (usbHostStat == SYS_STATUS_READY) {
                //USBHost is ready
                SYS_CONSOLE_PRINT("USB Host initialized\r\n");
                appData.state = APP_STATE_MOUNT_DRIVES;
            }
            break;
        }
        
        case APP_STATE_MOUNT_DRIVES:
        {   
            if ( /*appData.sdCardConnected &&*/ appData.usbDriveConnected) {
                appData.state = APP_STATE_TCPIP_WAIT_INIT;
            }
            
            break;
        }
        
        case APP_STATE_TCPIP_WAIT_INIT:
        {
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat < 0) {
                // some error occurred
                SYS_CONSOLE_PRINT("TCP/IP stack initialization failed!\r\n");
            }
            else if (tcpipStat == SYS_STATUS_READY) {
                //TCP/IP stack is ready
                HTTP_APP_Initialize();
                appData.state = APP_STATE_INIT_FINALIZE;
            }
            break;
        }
        
        case APP_STATE_INIT_FINALIZE:
        {
            bool appInitialized = true;

            if (appInitialized) {
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            
            if ((uint32_t)(millis()-timer) > 1000) {
                //usb_write();
                if (lcd_light) {
                    lcd_light = 0;
                }
                else {
                    lcd_light = 1;
                }
                timer = millis();
            }
            //static uint16_t i = 0;
            //SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Print from user task %d\r\n", i++);
            VS1003_handle();
            button_handle(&next_btn);
            
            int8_t tmp;
            if ( (tmp = rotary_handle()) ) {
                int8_t volume = VS1003_getVolume();
                volume += tmp;
                if (volume > 100) volume = 100;
                if (volume < 0) volume = 0;
                VS1003_setVolume(volume);                
            }
            
            lcd_ui_handle();
            lcd_handle();
            
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
    WDT_Clear();
}


/*******************************************************************************
 End of File
 */
