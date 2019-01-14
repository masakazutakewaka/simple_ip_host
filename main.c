#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<poll.h>
#include	<sys/ioctl.h>
#include	<netinet/ip_icmp.h>
#include	<netinet/if_ether.h>
#include	<linux/if.h>
#include	<arpa/inet.h>
#include	<sys/wait.h>
#include	<pthread.h>
#include	"sock.h"
#include	"ether.h"
#include	"arp.h"
#include	"ip.h"
#include	"icmp.h"
#include	"param.h"
#include	"cmd.h"

int	EndFlag=0;

int	DeviceSoc;

PARAM	Param;
