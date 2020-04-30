/*
    raw socket read sample.
    for linux, root.
    
    struct iphdr
    {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        unsigned int ihl:4;
        unsigned int version:4;
    #elif __BYTE_ORDER == __BIG_ENDIAN
        unsigned int version:4;
        unsigned int ihl:4;
    #else
        # error "Please fix <bits/endian.h>"
    #endif
        u_int8_t tos;
        u_int16_t tot_len;
        u_int16_t id;
        u_int16_t frag_off;
        u_int8_t ttl;
        u_int8_t protocol;
        u_int16_t check;
        u_int32_t saddr;
        u_int32_t daddr;
    };


    struct tcphdr
    {
        __extension__ union
        {
          struct
          {
            u_int16_t th_sport;             // source port
            u_int16_t th_dport;             // destination port
            tcp_seq th_seq;                 // sequence number
            tcp_seq th_ack;                 // acknowledgement number
    # if __BYTE_ORDER == __LITTLE_ENDIAN
            u_int8_t th_x2:4;               // (unused)
            u_int8_t th_off:4;              // data offset
    # endif
    # if __BYTE_ORDER == __BIG_ENDIAN
            u_int8_t th_off:4;              // data offset
            u_int8_t th_x2:4;               // (unused)
    # endif
            u_int8_t th_flags;
    # define TH_FIN 0x01
    # define TH_SYN 0x02
    # define TH_RST 0x04
    # define TH_PUSH        0x08
    # define TH_ACK 0x10
    # define TH_URG 0x20
            u_int16_t th_win;               /* window
            u_int16_t th_sum;               /* checksum 
            u_int16_t th_urp;               /* urgent pointer
          };
          
          struct
          {
            u_int16_t source;
            u_int16_t dest;
            u_int32_t seq;
            u_int32_t ack_seq;
    # if __BYTE_ORDER == __LITTLE_ENDIAN
            u_int16_t res1:4;
            u_int16_t doff:4;
            u_int16_t fin:1;
            u_int16_t syn:1;
            u_int16_t rst:1;
            u_int16_t psh:1;
            u_int16_t ack:1;
            u_int16_t urg:1;
            u_int16_t res2:2;
    # elif __BYTE_ORDER == __BIG_ENDIAN
            u_int16_t doff:4;
            u_int16_t res1:4;
            u_int16_t res2:2;
            u_int16_t urg:1;
            u_int16_t ack:1;
            u_int16_t psh:1;
            u_int16_t rst:1;
            u_int16_t syn:1;
            u_int16_t fin:1;
    # else
    #  error "Adjust your <bits/endian.h> defines"
    # endif
            u_int16_t window;
            u_int16_t check;
            u_int16_t urg_ptr;
          };
        };
    };
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() 
{

    struct sockaddr_in source_socket_address, dest_socket_address;
    int packet_size;

    // Allocate string buffer to hold incoming packet data
    unsigned char *buffer = (unsigned char *)malloc(65536);
    // Open the raw socket
    int sock = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock == -1)
    {
        //socket creation failed, may be because of non-root privileges
        perror("Failed to create socket");
        exit(1);
    }
    
    while(1) 
    {
        // recvfrom is used to read data from a socket
        packet_size = recvfrom(sock , buffer , 65536, 0 , NULL, NULL);
        if (packet_size == -1) 
        {
            printf("Failed to get packets\n");
            return 1;
        }

        struct iphdr *ip_packet = (struct iphdr *)buffer;
        int ip_header_size = (ip_packet->ihl * 4); // ip头的大小
        if(ip_packet->protocol == 6 /* is tcp */
            && packet_size > 40 && packet_size < 64)
        {
            struct tcphdr* tcp_packet = (struct tcphdr*)(buffer+ip_header_size);
            if(tcp_packet->syn != 1)
                continue;

            int tcp_option_len = tcp_packet->doff * 4 - sizeof(struct tcphdr); // tcp option 选项大小

            memset(&source_socket_address, 0, sizeof(source_socket_address));
            source_socket_address.sin_addr.s_addr = ip_packet->saddr;
            memset(&dest_socket_address, 0, sizeof(dest_socket_address));
            dest_socket_address.sin_addr.s_addr = ip_packet->daddr;
            printf("Incoming Packet: %d, ip_header_size: %d, tcp_option_len: %d\n", packet_size, ip_header_size, tcp_option_len);
            printf("Packet Size (bytes): %d\n",ntohs(ip_packet->tot_len));
            printf("Source Address: %s\n", (char *)inet_ntoa(source_socket_address.sin_addr));
            printf("Destination Address: %s\n", (char *)inet_ntoa(dest_socket_address.sin_addr));
            printf("Identification: %d\n", ntohs(ip_packet->id));

            printf("tcp port %d -> %d\n", ntohs(tcp_packet->source), ntohs(tcp_packet->dest) );

            
            for(int i=0; i< packet_size; i++)
            {
                printf("%X ", buffer[i]);
            }
            printf("\n\n");
        }

        // usleep(2000);
    }

    return 0;
}

