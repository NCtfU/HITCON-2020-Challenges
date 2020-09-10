#include "util.h"
/*
 * Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination. Terminates the string read
 * with a null character. If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null. If any of
 * the above three line terminators is read, the last character if the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * */

int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\x00';
    int n = 0;
    
    while ((i < size - 1) && (c != '\n')) {
        n = recv(sock, &c, 1, 0); 
        if (n > 0) {
            if (c == '\r') {
                n = recv(sock, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
        }
        buf[i] = c;
        i++;
    }
    buf[i] = '\x00';

    return i;
}

/* Return the informational HTTP headers about a file. */
void headers(int client, const char *filename)
{
    char buf[1024];
	/* could use filename to determine file type */
    (void)filename;

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}


/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. 
 * */
void die(const char *s)
{
    char prefix[] = "[!] ";
    strcat(prefix, s);
    perror(s);
    exit(1);
}
/* Inform the client that the requested web method has not
 * been implemented.
 * */
void unimplemented(const int client)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<html><head><title>Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</title></head>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<body><p>http request method not supported.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</body></html>\r\n");
    send(client, buf, strlen(buf), 0);
}
   

/* Give a client a 404 not found status message. */
void not_found(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 404 Not Found\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html><title>Not Found</title>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<body><p>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or non existent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</body></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

/* Inform the client that a request it has made has a problem */
void bad_request(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 400 Bad Request\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<p>Your browser sent a bad request, ");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(client, buf, sizeof(buf), 0);
}

/* Inform the client that a CGI script could not be executed */
void cannot_execute(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
	send(client, buf, strlen(buf), 0);
}





