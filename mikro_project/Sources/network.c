/*************************************************************************
/	Module		: Network-Functions for the TWR-K60N512
/-------------------------------------------------------------------------
/	Description	: Contains all functions which are related to the 
/				  network-support for TWR-K60N512
/
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz
/	Project		: TWR K60 N512 Mikroprozessorsysteme-Labor
/	Version		: 1.0
/	Released	: 01.04.2014
/	Written by	: Manuel Beha
/************************************************************************/

#include "main.h"
#include <ipcfg.h>
#include "network.h"

/*************************************************************************
/	Function        : init_network
/-------------------------------------------------------------------------
/	Description	:   This method is used to initialize the network stack.
/
/	Input Parameter : nothing
/
/	Output Parameter: FALSE	if anything went wrong;
/					  TRUE	if everything went fine.
/	Written by	: Manuel Beha
/   Date        : 01.04.2014
/*************************************************************************/
boolean init_network()
{
   IPCFG_IP_ADDRESS_DATA 	ip_data;
   _enet_address           	enet_address;   
   uint_32 error;
   
   _RTCS_socket_part_init = RTCS_NUMBER_OF_SOCKETS;
   _RTCS_socket_part_grow = 2;
   _RTCS_socket_part_max  = 20;
   error = RTCS_create();
   
   if (error != RTCS_OK) 
   {
      printf("init_network: RTCS failed to initialize, error = %X", error);
      _task_block();
   }

   _IP_forward = TRUE;
   
   ip_data.ip = ENET_IPADDR;
   ip_data.mask = ENET_IPMASK;
   // we dont need the gateway adress for the netgear-switch
   //ip_data.gateway = ENET_GATEWAY; 
   
   // calculate the mac-adress
   ENET_get_mac_address (ENET_DEVICE, ENET_IPADDR, enet_address);
   
   // initialize the device
   error = ipcfg_init_device (ENET_DEVICE, enet_address);
   if (error != RTCS_OK) 
   {
      printf("\nFailed to initialize ethernet device, error = %X", error);
      _task_block();
   }

#if RTCSCFG_ENABLE_LWDNS
   LWDNS_server_ipaddr = ENET_GATEWAY;   
   ipcfg_add_dns_ip(ENET_DEVICE,LWDNS_server_ipaddr);
#endif /* RTCSCFG_ENABLE_LWDNS */

 
   printf("Setting static IP address ... ");
   error = ipcfg_bind_staticip (ENET_DEVICE, &ip_data);
   

   if (error != IPCFG_ERROR_OK) 
   {
	   printf("\nRTCS failed to bind interface with IPv4, error = %X", error);
      _task_block();
   }
   else
	   printf("OK\n");

   ipcfg_get_ip(ENET_DEVICE, &ip_data);
   printf("IP Address      : %d.%d.%d.%d\n",IPBYTES(ip_data.ip));
   printf("Subnet Address  : %d.%d.%d.%d\n",IPBYTES(ip_data.mask));
   printf("Gateway Address : %d.%d.%d.%d\n",IPBYTES(ip_data.gateway));
   printf("DNS Address     : %d.%d.%d.%d\n",IPBYTES(ipcfg_get_dns_ip(ENET_DEVICE,0)));
   printf("MAC Address     : %02x:%02x:%02x:%02x:%02x:%02x\n",
		   	   	   	   	   	   enet_address[0], enet_address[1], enet_address[2], 
		   	   	   	   	   	   enet_address[3], enet_address[4], enet_address[5]);


   return TRUE;
}

