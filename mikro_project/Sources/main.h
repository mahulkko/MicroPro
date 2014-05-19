#ifndef __main_h_
#define __main_h_
#include <mqx.h>
#include <bsp.h>




#include <rtcs.h>
#ifdef APPLICATION_HAS_SHELL
#include <sh_rtcs.h>
#endif
#include <ipcfg.h>



#define MAIN_TASK   1

#define ENET_DEVICE 0
#define RTCS_DHCP 0


extern void Main_task (uint_32);

void recv_task();


/* PPP device must be set manually and 
** must be different from the default IO channel (BSP_DEFAULT_IO_CHANNEL) 
*/
#define PPP_DEVICE      "ittyb:" 

/*
** Define PPP_DEVICE_DUN only when using PPP to communicate
** to Win9x Dial-Up Networking over a null-modem
** This is ignored if PPP_DEVICE is not #define'd
*/
#define PPP_DEVICE_DUN  1



#endif /* __main_h_ */

