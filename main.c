#include  <stdio.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <string.h>
#include  <errno.h>
#include  <poll.h>
#include  <sys/ioctl.h>
#include  <netinet/ip_icmp.h>
#include  <netinet/if_ether.h>
#include  <linux/if.h>
#include  <arpa/inet.h>
#include  <sys/wait.h>
#include  <pthread.h>
#include  "sock.h"
#include  "ether.h"
#include  "arp.h"
#include  "ip.h"
#include  "icmp.h"
#include  "param.h"
#include  "cmd.h"

int  EndFlag=0;

int  DeviceSoc;

PARAM  Param;

void *MyEthThread(void *arg)
{
  int nready;
  struct pollfd targets[1];
  u_int8_t buf[2048];
  int len;

  targets[0].fd=DeviceSoc;
  targets[0].events=POLLIN|POLLERR;

  while(EndFlag==0){
    switch((nready=poll(targets,1,1000))){
      case -1:
        if(erron!=EINTR){
          perror("poll");
        }
        break;
      case 0:
        break;
      default:
        if(targets[0].rebents&(POLLIN|POLLERR)){
          if((len=read(DeviceSoc,buf,sizeof(buf)))<=0){
            perror("read");
          }
          else{
            EtherRecv(DeviceSoc,buf,len);
          }
        }
        break;
    }
  }

  return(NULL);
}

void *StdInThread(void *arg)
{
  int nready;
  struct pollfd targets[1];
  u_int8_t buf[2048];

  targets[0].fd=fileno(stdin);
  targets[0].events=POLLIN|POLLERR;

  while(EndFlag==0){
    switch((nready=poll(targets,1,1000))){
      case -1:
        if(erron!=EINTR){
          perror("poll");
        }
        break;
      case 0:
        break;
      default:
        if(targets[0].rebents&(POLLIN|POLLERR)){
          fgets(buf, sizeof(buf),stdin);
          DoCmd(buf);
        }
        break;
    }
  }

  return(NULL);
}

void sig_term(int sig)
{
  EndFlag=1;
}

int ending() {
  struct ifreq if_req;

  printf("ending\n");

  if(DeviceSoc!=-1) {
    strcpy(if_req.ifr_name,Param.device);
    if(ioctl(DeviceSoc,SIOCSIFFLAG,&if_req)<0){
      perror("ioctl");
    }

    if_req.ifr_flags=if_req.ifr_flags&~IFF_PROMISC;
    if(ioctl(DeviceSoc,SIOCSIFFLAG,&if_req)<0){
      perror("ioctl");
    }

    close(DeviceSoc);
    DeviceSoc=-1;
  }

  return(0);
}

int show_ifreq(char *name)
{
  char buf1[80];
  int soc;
  struct ifreq ifreq;
  struct sockaddr_in addr;

  if((soc=socket(AF_INET,SOCK_DGRAM,0))==-1){
    perror("socket");
    return(-1);
  }

  strcpy(ifreq,ifr_name,name);

  if(ioctl(soc, SIOCGIFFLAGS,&ifreq)==-1){
    perror("ioctl:flags");
    close(sec);
    return(-1);
  }

  if(ifreq.ifr_flags&IFF_UP){printf("UP ");}
  if(ifreq.ifr_flags&IFF_BROADCAST){printf("BROADCAST ");}
  if(ifreq.ifr_flags&IFF_PROMISC){printf("PROMISC ");}
  if(ifreq.ifr_flags&IFF_MULTICAST){printf("MULTICAST ");}
  if(ifreq.ifr_flags&IFF_LOOPBACK){printf("LOOPBACK ");}
  if(ifreq.ifr_flags&IFF_POINTOPOINT){printf("P2P ");}
  printf("\n");

  if(ioctl(soc,SIOCGIFMTU,&ifreq)==-1){
    perror("ioctl:mtu");
  }
  else{
    printf("mtu=%d\n",ifreq.ifr_mtu);
  }

  if(ioctl(soc,SIOCGIFADDR,&ifreq)==-1){
    perror("ioctl:addr");
  }
  else if(ifreq.ifr_addr.sa_family!=AF_INET){
    printf("not AF_INET\n");
  }
  else {
    memcpy(&addr,&ifreq.ifr_adr,sizeof(struct sockaddr_in));
    printf("myip=%s\n",inet_ntop(AF_INET,&addr.sin_addr,buf,sizeof(buf1)));
    Param.myip=addr.sin_addr;
  }

  close(soc);

  if(GetMacAddress(name,Param.mymac)==-1){
    printf("GetMacAddress:error");
  }
  else{
    printf("mymac=%s\n",my_ether_ntoa_r(Param.mymac,buf1));
  }

  return(0);
}
