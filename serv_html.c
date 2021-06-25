#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#define SIZE 1024
#define BACKLOG 10 //num of connections

void report(struct sockaddr_in *serverAddress);

void setHttpHeader(char httpHeader[])
{
    FILE *htmlData = fopen("index.html", "r");

    char line[100];
    char responseData[8000];
    while (fgets(line, 100, htmlData) != 0) {
        strcat(responseData, line);
    }
    strcat(httpHeader, responseData);
}

int main()
{
    char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    // AF_INET == ip4
    // SOCK_STREAM, *_DGRAM == tcp,udp
    int serverSocket = socket(AF_INET,SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8001);
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//inet_addr("127.0.0.1");

    bind(serverSocket,(struct sockaddr *) &serverAddress,sizeof(serverAddress));

    int listening = listen(serverSocket, BACKLOG);
    if (listening < 0) {
        printf("Error: The server is not listening.\n");
        return 1;
    }
    report(&serverAddress);     // report function
    setHttpHeader(httpHeader);  // header setup
    int clientSocket;

    // my looper
    while(1) {
        clientSocket = accept(serverSocket, NULL, NULL);
        send(clientSocket, httpHeader, sizeof(httpHeader), 0);
        close(clientSocket);
    }
    return 0;
}

void report(struct sockaddr_in *serverAddress)
{
    char host_set[INET6_ADDRSTRLEN]; // host setup
    char service_set[NI_MAXSERV]; // defined in `<netdb.h>`
    socklen_t addr_len = sizeof(*serverAddress);
    int err = getnameinfo(
        (struct sockaddr *) serverAddress,
        addr_len,
        host_set,
        sizeof(host_set),
        service_set,
        sizeof(service_set),
        NI_NUMERICHOST
    );
    if (err != 0) {
        printf("It's not working!!\n");
    }
    printf("\n\n\tServer listening on http://%s:%s\n", host_set, service_set);
}