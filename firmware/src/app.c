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
#include "app.h"
#include "system_config.h"
#include "system_definitions.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usb/usb_host.h"
#include "usb/usb_host_msd.h"
#include "vs1053/vs1053.h"
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
#include "stream_list.h"

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
    SYS_STATUS tcpipStat;
    SYS_STATUS usbHostStat;
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            SYS_CONSOLE_PRINT("Initializing user app\r\n");
            SYS_CONSOLE_PRINT("RCON: %d\r\n", RCON);
            RCON = 0;
            lcd_init();
            lcd_cls();
            ui_init();          
            
            VS1053_init();
            VS1053_setVolume(50);
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
                // USB initialized - read list of streams and find max
                initialize_stream_list();
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
            VS1053_handle();   
            ui_handle();
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
