#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

#include "util.h"

#define ISspace(x) isspace((int)x)

void execute_cgi(int client, const char *path, const char *method, const char *query_string)
{
    char buf[1024];
    int cgi_output[2];
    int cgi_input[2];
    int numchars = 1;
    int content_length = -1;
    int status;

    buf[0] = 'A'; buf[1] = '\x00';
    if (strcasecmp(method, "GET") == 0) {
        /* read & discard headers */
        while ((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(client, buf, sizeof(buf));
    } else {
        /* POST */
        numchars = get_line(client, buf, sizeof(buf));
#ifdef DEBUG
        printf("[>] %s \n", buf);
#endif
        while ((numchars > 0) && strcmp("\n", buf)) {
            buf[15] = '\x00';
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = get_line(client, buf, sizeof(buf));
#ifdef DEBUG
            printf("[>] %s", buf);
#endif
        }
        /* if http header doesn't indicate the body size,
         * it is a bad request
         * */
        if (content_length < 0) {
            bad_request(client);
            return;
        }
    }
    
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);

    if (pipe(cgi_output) < 0) {
        cannot_execute(client);
        return;
    }
    
    if (pipe(cgi_input) < 0) {
        cannot_execute(client);
        return;
    }

    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        cannot_execute(client);
        return;
    }

    /* Using child process to exec cgi */
    if (pid == 0) {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        dup2(cgi_output[1], 1);
        dup2(cgi_input[0], 0);
        close(cgi_output[0]);
        close(cgi_input[1]);
        /* CGI env */
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);
        if (strcasecmp(method, "GET") == 0) {
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        } else {
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        execl(path, path, NULL);
        exit(0);
    } else {
        /* Parent */
        close(cgi_output[1]);
        close(cgi_input[0]);
        char c;
        if (strcasecmp(method, "POST") == 0) {
            for (int i = 0; i < content_length; i++) {
                recv(client, &c, 1, 0);
#ifdef DEBUG
                printf("%c", c);
#endif 
                write(cgi_input[1], &c, 1);
            }
#ifdef DEBUG
            printf("\n");
#endif 
        }
        while (read(cgi_output[0], &c, 1) > 0) {
#ifdef DEBUG
            printf("%c", c);
#endif 
            send(client, &c, 1, 0);
        }
#ifdef DEBUG
        printf("\n");
#endif 
        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);
    }
}

/* 
 * Put the entire contents of a file out on a socket. This function
 * is anmed after the UNIX "cat" cmd, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * */
void cat(int client, FILE *resource)
{
	char buf[1024];
    fgets(buf, sizeof(buf), resource);
    while (!feof(resource)) {
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
}

/*
 * Send a regular file to the client. Use headers, and report
 * errors to client if they occur.
 * */
void serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[1024];
    
    /* make sure buf has something to into loop */
    buf[0] = 'A';
    buf[1] = '\x00';

    while ((numchars > 0) && strcmp("\n", buf))
        numchars = get_line(client, buf, sizeof(buf));

    resource = fopen(filename, "r");
    if (resource == NULL)
        not_found(client);
    else {
        headers(client, filename);
        cat(client, resource);
    }
    fclose(resource);
}

   
/* A request has caused a call to accept() on the server port to 
 * return. Process the request qppropriately.
 * */
void accept_request(const int client)
{
    char buf[1024];
    char method[255];
    char url[255];
    char path[512];
    size_t i = 0, j = 0;
    /* becomes true if server decides the is a cgi program */
    int cgi = 0;
    struct stat st;
    char *query_string = NULL;


    int numchars = get_line(client, buf, sizeof(buf));

#ifdef DEBUG
    printf("[D] request : %s", buf);
#endif

    /* First line is request line like POST GET .... methods */
    while (!ISspace(buf[j]) && (i < sizeof(method) - 1)) {
        method[i] = buf[j];
        i++; j++;
    }
    method[i] = '\x00';

    if (strcasecmp(method, "GET") && strcasecmp(method, "POST")) {
        unimplemented(client); 
        return;
    }

    if (strcasecmp(method, "POST") == 0)
        cgi = 1;

    i = 0;
    /* bypass all blank-space */
    while (ISspace(buf[j]) && (j < sizeof(buf)))
        j++;

    /* parse URL */
    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf))) {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\x00';

    if (strcasecmp(method, "GET") == 0) {
        query_string = url;

        /*
         * travesing the URL to find the ? it is 
         * indicated that we need cgi to handle the
         * paramters.
         * */
        while ((*query_string != '?') && (*query_string != '\x00'))
            query_string++;
        
        if (*query_string == '?') {
            cgi = 1;
            *query_string = '\x00';
            query_string++;
        }
    }

    sprintf(path, "htdocs%s", url);
    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");

#ifdef DEBUG
    printf("[D] request file : %s \n", path);
#endif
    
    if (stat(path, &st) == -1) {
        /* if the htdocs/index.html not found */
        while ((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(client, buf, sizeof(buf));
        not_found(client);
    } else {
        /* if this file is a dir */
	    if ((st.st_mode & S_IFMT) == S_IFDIR)	
            strcat(path, "/index.hmtl");

        /* if this file is an executable file */
        if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)) {
            cgi = 1; 
        }

        if (!cgi)
            serve_file(client, path);
        else
            execute_cgi(client, path, method, query_string);
	}
    close(client);
}

int startup(u_short *port)
{
    struct sockaddr_in name;

    int httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
        die("socket");

    if (*port == 0)
        *port = 10201;

    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    {
        int opt = 1;
        int opt_len = sizeof(opt);
        setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len);
    }

    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        die("bind");

    if (listen(httpd, 5) < 0)
        die("listen");

    return httpd;
}

int main()
{
	int server_sock = -1;
    int client_sock = -1;
	struct sockaddr_in client_name;
    int client_name_len = sizeof(client_name);
    u_short port = 0;

    server_sock = startup(&port);
    printf("[+] httpd running on port %d\n", port);
	
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
        if (client_sock == -1) {
            die("accept");
        }
        accept_request(client_sock) ;   
    }

    close(server_sock);
    return 0;
	
}
