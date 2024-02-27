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


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define SERV_PORT 7777
#define CLIENT_PORT 5555


int main() {
    int fd;
    struct sockaddr_in serv_addr;
    socklen_t serv_size;
    struct in_addr serv_ip_adress;
    char *string_data = NULL;


    struct udphdr *udp_hdr = NULL;
    char buffer[1024] = {0};
    char msg[] = "hello";

    char recv_buf[128], *recv_string_data = NULL;
    
    
    
    struct iphdr *ip_hdr = NULL;
    int flag = 1;


    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) handle_error("socket");

    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(int)) == -1) handle_error("setsockopt");



    // SERV_STRUCT
    if (inet_pton(AF_INET, "127.0.0.1", &(serv_ip_adress.s_addr) ) != 1) handle_error("inet_pton");


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr = serv_ip_adress;

    serv_size = sizeof(serv_addr);


    //IP HEADER
    ip_hdr = (struct iphdr *) buffer;



    ip_hdr->version = 4;
    ip_hdr->ihl = 5;
    ip_hdr->tos = 0;
    // ip_hsdr->tot_len;  // always filled in 
    ip_hdr->id = htons(0);       // filled when zero
    ip_hdr->frag_off = htons(0);
    ip_hdr->ttl = 1;
    ip_hdr->protocol = IPPROTO_UDP;
    // ip_hdr->check;   // always filled in
    ip_hdr->saddr = htons(0);   // filled when zero
    ip_hdr->daddr = inet_addr("127.0000.0000.0001");  // 0x7F000001 - 127.0.0.1

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



    if (sendto(fd, buffer, (sizeof(struct iphdr) + sizeof(struct udphdr)+strlen(string_data)+1), 0, (struct sockaddr *) &serv_addr, serv_size) == -1) handle_error("setdto");
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




