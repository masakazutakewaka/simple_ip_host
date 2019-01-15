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
