#include "../../includes/px_botnet.h"

char* host_to_ip(const char* hostname){
  struct hostent *host_entry;
  host_entry=gethostbyname(hostname);
  if(host_entry){
    return inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
  }
  else return NULL;
}

bool http_create_socket(SOCKET *sock, SOCKADDR_IN *sin, const char *ip){
  *sock = socket(AF_INET, SOCK_STREAM, 0);		//init the socket

  sin->sin_addr.s_addr = inet_addr(ip);			//init the socket address on ip / port / network
  sin->sin_port = htons(PORT);
  sin->sin_family = AF_INET;

  if(connect(*sock, (SOCKADDR*)sin, sizeof(*sin)) == SOCKET_ERROR)return false;

#ifdef linux
  fcntl(*sock, F_SETFL, O_NONBLOCK);
#elif defined WIN32
  u_long mode=1;
  ioctlsocket(*sock,FIONBIO,&mode);
#endif

  return true;

}

char* http_request(SOCKET sock, const char *hostname, const char *page){
  char buf[BUFSIZ];
  int len = 0;
  int selection;

  //Create a GET HTTP request
  len = sprintf(buf, "\
GET %s %s\r\n\
Host: %s\r\n\
%s\r\n\
\r\n",
		page, HTTP_VERSION,
		hostname,
		CONNECTION_TYPE);

  //send the http request
  send(sock, buf, strlen(buf), 0);

  // timeout
  struct timeval tv;
  tv.tv_sec = TIMEOUT_SEC;
  tv.tv_usec = TIMEOUT_MSEC;

  fd_set fdread;
  FD_ZERO(&fdread);
  FD_SET(sock, &fdread);

  // the result is the remote page content.
  // will be reallocated at each call of recv
  char *result = malloc(sizeof(char));
  result[0] = '\0';

  while(1){
    selection = select(sock+1, &fdread, NULL, NULL, &tv);

    if (!selection || !(FD_ISSET(sock, &fdread))){
      break;
    }
    else{
      len = recv(sock, buf, BUFSIZ, 0);
      if(len == 0)break;
      result = realloc(result, (strlen(result) + len + 1) * sizeof(char));
      strncat(result, buf, len);
    }
  }

  return result;
}

char* http_header_strip(char* content){
  return strstr(content, "\r\n\r\n")+4;
}

char* https_request(const char* hostname, const char* page) {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);

    SSL* ssl = SSL_new(ctx);

    BIO* bio_conn = BIO_new_ssl_connect(ctx);
    BIO* bio_mem = BIO_new(BIO_s_mem());

    SSL* ssl_conn;
    BIO_get_ssl(bio_conn, &ssl_conn);
    SSL_set_mode(ssl_conn, SSL_MODE_AUTO_RETRY);

    char hostname_with_port[BUFSIZ];
    snprintf(hostname_with_port, BUFSIZ, "%s:%d", hostname, PORT);
    BIO_set_conn_hostname(bio_conn, hostname_with_port);

    if (BIO_do_connect(bio_conn) <= 0) {
        // Handle failed connection
        fprintf(stderr, "Failed to establish connection.\n");
        ERR_print_errors_fp(stderr);
        return NULL;
    } else {
        printf("Connected to %s\n", hostname_with_port);
    }

    char request[BUFSIZ];
    snprintf(request, BUFSIZ, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", page, hostname);
    printf("Request: %s\n", request);

    BIO_write(bio_conn, request, strlen(request));

    char* response = malloc(BUFSIZ);
    response[0] = '\0';

    char buf[BUFSIZ];
    int len;

    while ((len = BIO_read(bio_conn, buf, BUFSIZ - 1)) > 0) {
        buf[len] = '\0';  // Null-terminate the received data
        response = realloc(response, strlen(response) + len + 1);
        strcat(response, buf);
    }

    BIO_free_all(bio_conn);
    SSL_CTX_free(ctx);
    printf("Response: %s\n", response);
    return response;
}