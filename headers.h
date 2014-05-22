#ifndef _SOCKET_HEADERS_H_
#define _SOCKET_HEADERS_H_

#define INTERFACE "eth0"

struct ip {
    unsigned int    ip_length:4;  // little-endian
    unsigned int    ip_version:4;
    unsigned char   ip_tos;
    unsigned short   ip_total_length;
    unsigned short   ip_id;
    unsigned short   ip_flags;
    unsigned char   ip_ttl;
    unsigned char   ip_protocol;
    unsigned short  ip_cksum;
    unsigned int    ip_source;
    unsigned int    ip_dest;
};

struct tcp {
    unsigned short  tcp_source_port;
    unsigned short  tcp_dest_port;
    unsigned int    tcp_seqno;
    unsigned int    tcp_ackno;
    unsigned int    tcp_res1:4,  // little-endian
                    tcp_hlen:4,
                    tcp_fin:1,
                    tcp_syn:1,
                    tcp_rst:1,
                    tcp_psh:1,
                    tcp_ack:1,
                    tcp_urg:1,
                    tcp_res2:2;
    unsigned short  tcp_winsize;
    unsigned short  tcp_cksum;
    unsigned short  tcp_urgent;
};

struct udp {
    unsigned short udp_source_port;
    unsigned short udp_dest_port;
    unsigned short udp_length;
    unsigned short udp_cksum;
};

#endif // _SOCKET_HEADERS_H_
