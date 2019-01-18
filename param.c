#include  <stdio.h>
#include  <ctype.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <string.h>
#include  <errno.h>
#include  <signal.h>
#include  <time.h>
#include  <fcntl.h>
#include  <sys/stat.h>
#include  <sys/param.h>
#include  <sys/types.h>
#include  <sys/ioctl.h>
#include  <sys/socket.h>
#include  <arpa/inet.h>
#include  <netpacket/packet.h>
#include  <net/ethernet.h>
#include  <netinet/if_ether.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <sys/wait.h>
#include  <netinet/in.h>
#include  <netinet/ip.h>
#include  <netinet/ip6.h>
#include  "sock.h"
#include  "ether.h"
#include  "param.h"

extern PARAM Param;

static char ParamFname=NULL;

int SetDefaultParam()
{
  Param.MTU=DEFAULT_MTU;
  Param.IpTTL=DEFAULT_IP_TTL;

  return(0);
}
