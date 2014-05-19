/*************************************************************************
/	Module		: Network-Functions for the TWR-K60N512
/-------------------------------------------------------------------------
/	Description	: Contains all functions and settings which are related to 
/				  the network-support for TWR-K60N512
/
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz
/	Project		: TWR K60 N512 Mikroprozessorsysteme-Labor
/	Version		: 1.0
/	Released	: 01.04.2014
/	Written by	: Manuel Beha
/************************************************************************/

#ifndef NETWORK_H_
#define NETWORK_H_

// network settings
#define ENET_IPADDR IPADDR(192,168,1,96)
#define ENET_IPMASK IPADDR(255,255,255,0)
#define ENET_GATEWAY IPADDR(192,168,1,1)
#define RTCS_PPP 0

#define RTCS_NUMBER_OF_SOCKETS			10
#define RTCS_NUMBER_OF_SOCKETS_GROW		2
#define RTCS_NUMBER_OF_SOCKETS_MAX		50


boolean init_network();


#endif /* NETWORK_H_ */
