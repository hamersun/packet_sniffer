#include <stdio.h>
#include <sys/socket.h>
//#include <socketbits.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "headers.h"

int open_raw_socket(void);
int set_promisc(char *interface, int sock);

int main(int argc, char *argv[])
{
    int sock, bytes_received, fromlen;
    char buffer[65535];
    struct sockaddr_in from;
    struct ip *ip;
    struct tcp *tcp;
    struct udp *udp;

    sock = open_raw_socket();

    set_promisc(INTERFACE, sock);

    while(1) {
        fromlen = sizeof from;
        bytes_received = recvfrom(sock, buffer, sizeof buffer, 0,
                (struct sockaddr *)&from, &fromlen);
        printf("\nBytes received ::: %5d\n", bytes_received);
        printf("Source address ::: %s\n", inet_ntoa(from.sin_addr));

        ip = (struct ip *)buffer;
        /* see if this is a TCP packet */
        if (ip->ip_protocol == 6) {
            /* This is a TCP packet */
            printf("IP header length ::: %d\n", ip->ip_length);
            printf("Protocol ::: %d\n", ip->ip_protocol);
            tcp = (struct tcp *) (buffer + 4*ip->ip_length);
            printf("Source port ::: %d\n", ntohs(tcp->tcp_source_port));
            printf("Dest port ::: %d\n", ntohs(tcp->tcp_dest_port));
        } else if (ip->ip_protocol == 17) {
            /* This is a UDP packet */
            printf("IP header length :: %d\n", ip->ip_length);
            printf("Protocol ::: %d\n", ip->ip_protocol);
            udp = (struct udp *) (buffer + 4*ip->ip_length);
            printf("Source port ::: %d\n", ntohs(udp->udp_source_port));
            printf("Dest port ::: %d\n", ntohs(udp->udp_dest_port));
            printf("UDP length ::: %d\n", ntohs(udp->udp_length));
            printf("UDP checksum ::: 0x%x\n", ntohs(udp->udp_cksum));
        }
    }
}

int open_raw_socket()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        perror("The raw socket was not created");
        exit(0);
    }
    return (sock);
}

int set_promisc(char *interface, int sock)
{
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, strlen(interface)+1);
    if ((ioctl(sock, SIOCGIFFLAGS, &ifr) == -1)) {
        /* Could not retrieve flags for the interface */
        perror("Could not retrive flags for the interface");
        exit(0);
    }
    printf("The interface is ::: %s\n", interface);
    printf("Retrieved flags from interface successfully\n");

    /* now that the flags have been retrieved */
    /* set the flags to PROMISC */
    ifr.ifr_flags |= IFF_PROMISC;
    if (ioctl(sock, SIOCSIFFLAGS, &ifr) == -1) {
        /* Could not set the flags on the interface */
        perror("Could not set the PROMISC flag:");
        exit(0);
    }
    printf("Setting interface ::: %s ::: to promisc", interface);
    return 0;
}

