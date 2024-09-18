#include "csapp.h"
#include "sbuf.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define NTHREADS 4
#define SBUFSIZE 16

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
sbuf_t sbuf;
int cachecnt;

typedef struct uri_t
{
    char hostname[MAXLINE];
    char port[MAXLINE];
    char path[MAXLINE];
} uri_t;

void doit(int fd);
void parse_uri(char *uri, uri_t *uri_parsed);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg); 
void generate_header(char *header, uri_t *uri_parsed, rio_t *rio);
void modify_header(char *buf, int *connection_exist, int *proxy_connection_exist); 
void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    pthread_t tid;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    listenfd = Open_listenfd(argv[1]);

    /* Initialize the thread pool */
    sbuf_init(&sbuf, SBUFSIZE);
    for (int i = 0; i < NTHREADS; ++i) {
        /* Creates worker threads */
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        sbuf_insert(&sbuf, connfd);
    }
    return 0;
}

void *thread(void *vargp) {
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf);
        doit(connfd);
        Close(connfd);
    }
}

void doit(int fd) {
    int proxyfd;
    char buf_server[MAXLINE], buf_client[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char header[MAXLINE];
    rio_t rio_client, rio_server;

    /* Read request line and headers */
    Rio_readinitb(&rio_client, fd);
    if (!Rio_readlineb(&rio_client, buf_client, MAXLINE)) {
        return;
    }
    printf("%s", buf_client);
    sscanf(buf_client, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented", "Proxy does not implement this method");
        return;
    }

    /* Parse uri into three part */    
    struct uri_t *uri_parsed = (struct uri_t *)Calloc(1, sizeof(struct uri_t));
    parse_uri(uri, uri_parsed);

    /* Generate the header and send it to the server */ 
    generate_header(header, uri_parsed, &rio_client);

    // P(&mutex_call_server);
    /* Open the server and send the new request to it */
    proxyfd = Open_clientfd(uri_parsed->hostname, uri_parsed->port); 
    Rio_readinitb(&rio_server, proxyfd);

    // //todo: Correct the generate_header function
    // printf("new header is %s\n", header);
    
    Rio_writen(proxyfd, header, strlen(header));

    /* Send back to the client */
    size_t n;
    while((n = Rio_readlineb(&rio_server, buf_server, MAXLINE)) != 0) {
        printf("server sent back %d bytes\n", (int)n);
        Rio_writen(fd, buf_server, n);
    }
    // V(&mutex_call_server);
    
    Close(proxyfd);
}

void parse_uri(char *uri, uri_t *uri_parsed) {
    // example: http://www.cmu.edu:8080/hub/index.html
    // => hostname = www.cmu.edu, port = 8080, path = /hub/index.html
    char *ptr  = strstr(uri, "//"), *portptr, *pathptr;
    if (ptr == NULL) {
        return;
    } else {
        ptr += 2;
    }
    pathptr = strchr(ptr, '/');
    if ((portptr = strchr(ptr, ':')) != NULL) {
        int port;
        sscanf(portptr + 1, "%d", &port);
        sprintf(uri_parsed->port, "%d", port);
        strncpy(uri_parsed->hostname, ptr, portptr - ptr);
    } else {
        strcpy(uri_parsed->port, "80");
        strncpy(uri_parsed->hostname, ptr, pathptr - ptr);
    }
    strcat(uri_parsed->hostname, "\0");
    strcpy(uri_parsed->path, pathptr);
}


void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Proxy Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>Proxy</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
} 

void generate_header(char *header, uri_t *uri_parsed, rio_t *rio) {
    strcpy(header, "GET ");
    strcat(header, uri_parsed->path);
    strcat(header, " HTTP/1.0\r\n");
    strcat(header, user_agent_hdr);
    int connection_exist = 0, proxy_connection_exist = 0;

    char buf[MAXLINE];
    do {
        Rio_readlineb(rio, buf, MAXLINE);
        if (!strcmp(buf, "\r\n")) {
            if (!connection_exist) {
                strcat(header, "Connection: close\r\n");
            }
            if (!proxy_connection_exist) {
                strcat(header, "Proxy-Connection: close\r\n");
            }
        }
        modify_header(buf, &connection_exist, &proxy_connection_exist); 
        strcat(header, buf);
    } while (strcmp(buf, "\r\n"));

}

/**
 * Change the Connection and Proxy-Connection header
 */
void modify_header(char *buf, int *connection_exist, int *proxy_connection_exist) {
    char *find_semicolon = strchr(buf, ':');
    if (find_semicolon != NULL) {
        char buf_head[MAXLINE];
        strncpy(buf_head, buf, find_semicolon - buf);
        strcat(buf_head, "\0");
        if (!strcmp(buf_head, "Connection")) {
            *connection_exist = 1;
            strcpy(find_semicolon, ": close\r\n");
        } else if (!strcmp(buf_head, "Proxy-Connection")) {
            *proxy_connection_exist = 1;
            strcpy(find_semicolon, ": close\r\n");
        } 
    }
}