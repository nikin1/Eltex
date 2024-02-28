#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>


#include <arpa/inet.h>
#include <linux/ip.h>



#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */


#include <net/if.h> //if_nametoindex



#define SERV_PORT 7777
#define CLIENT_PORT 5555
#define MY_INTERFACE "wlp1s0"


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


struct macheader {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short type;
};

int check_sum(char *buf) {
    int csum = 0;
    short *ptr;

    ptr = (short *) buf;

    for (int i = 0; i < 20; i++) {
        csum = csum + *ptr; // maybe + *ptr ??? 
        ptr++;
    }

    int tmp = csum >> 16;
    csum = (csum & 0xFFFF) + tmp;
    csum = ~csum;

    return csum;
}


int main() {
    int fd;
    struct sockaddr_ll serv_addr;
    socklen_t serv_size;
    struct in_addr serv_ip_adress;
    char *string_data = NULL;


    struct udphdr *udp_hdr = NULL;
    char buffer[1024] = {0};
    char msg[] = "hello";

    char recv_buf[128], *recv_string_data = NULL;
    
    
    
    struct iphdr *ip_hdr = NULL;

    struct macheader *mac_hdr = NULL;

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) handle_error("socket");




    // SERV_STRUCT
    if (inet_pton(AF_INET, "127.0.0.1", &(serv_ip_adress.s_addr) ) != 1) handle_error("inet_pton");


    // memset(&serv_addr, 0, sizeof(serv_addr));
    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(SERV_PORT);
    // serv_addr.sin_addr = serv_ip_adress;

    serv_size = sizeof(serv_addr);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sll_family = AF_PACKET;
    serv_addr.sll_ifindex = if_nametoindex(MY_INTERFACE);
    serv_addr.sll_halen = 6;
    // serv_addr.sll_addr;
    // strcat(serv_addr.sll_addr, "2cf05d436ffd");  //mac-adress
    // 2c:f0:5d:43:6f:fd
    serv_addr.sll_addr[0] = 0xfd;
    serv_addr.sll_addr[1] = 0x6f;
    serv_addr.sll_addr[2] = 0x43;
    serv_addr.sll_addr[3] = 0x5d;
    serv_addr.sll_addr[4] = 0xf0;
    serv_addr.sll_addr[5] = 0x2c;



    // 2c:f0:5d:43:6f:fd - MY SERV_MAC
    // 84:c8:a0:51:54:f0 - MY CLIENT_MAC

    // MAC HEADER
    mac_hdr = (struct macheader *) buffer;
    mac_hdr->src_mac[0] = 0xf0;
    mac_hdr->src_mac[1] = 0x54;
    mac_hdr->src_mac[2] = 0x51;
    mac_hdr->src_mac[3] = 0xa0;
    mac_hdr->src_mac[4] = 0xc8;
    mac_hdr->src_mac[5] = 0x84;


    mac_hdr->dest_mac[0] = 0xfd;
    mac_hdr->dest_mac[1] = 0x6f;
    mac_hdr->dest_mac[2] = 0x43;
    mac_hdr->dest_mac[3] = 0x5d;
    mac_hdr->dest_mac[4] = 0xf0;
    mac_hdr->dest_mac[5] = 0x2c;


    mac_hdr->type = ETHERTYPE_IP; // IPv4 - 0x800





    //IP HEADER
    ip_hdr = (struct iphdr *) buffer;

    // add zero !!!!
    int total_lenght = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(string_data) + 1;

    ip_hdr->version = 4;
    ip_hdr->ihl = 5;
    ip_hdr->tos = 0;
    ip_hdr->tot_len = htons(total_lenght) ; 
    ip_hdr->id = htons(0);
    ip_hdr->frag_off = htons(0);
    ip_hdr->ttl = 1;
    ip_hdr->protocol = IPPROTO_UDP;
    ip_hdr->saddr = inet_addr("127.0000.0000.0001");
    ip_hdr->daddr = inet_addr("127.0000.0000.0001");  // 0x7F000001 - 127.0.0.1
    ip_hdr->check = htons(0);



    int csum = check_sum(buffer);
    printf("csum: %d\n", csum);
    ip_hdr->check = csum;   // always filled in



    // UDP HEADER
    udp_hdr = (struct updhdr *) (buffer + sizeof(struct iphdr));

    udp_hdr->source = htons(CLIENT_PORT);
    udp_hdr->dest = htons(SERV_PORT);
    udp_hdr->len = htons(sizeof(struct udphdr) + strlen(msg));
    udp_hdr->check = htons(0);

    // printf("checksum is %x\n", udp_hdr->check);




    /* copy the data after the UDP header */
    string_data = (char *) (buffer + sizeof(struct iphdr) + sizeof(struct udphdr));
    strncpy(string_data, msg, strlen(msg));

    // printf("string_data: %s\n", string_data);



    if (sendto(fd, buffer, total_lenght, 0, (struct sockaddr *) &serv_addr, serv_size) == -1) handle_error("setdto");
    sleep(1);
  
    while (1) {
        if (recvfrom(fd, recv_buf, (sizeof(struct iphdr) + sizeof(struct udphdr)+strlen(string_data)+1), 0, NULL, NULL) == -1) handle_error("recfrom");
        udp_hdr = (struct udphdr *)(recv_buf + sizeof(struct iphdr));

        if (CLIENT_PORT == ntohs(udp_hdr->dest)) {
            // printf("udp_hdr->dest: %d\n", udp_hdr->dest);
            // printf("our port\n");


            recv_string_data = (char *) (recv_buf + sizeof(struct iphdr) + sizeof(struct udphdr));
            printf("msg: %s\n", recv_string_data);
        }
    }
    

    close(fd);
    return 0;
}




