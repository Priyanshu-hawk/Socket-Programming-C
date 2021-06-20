#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>


int main(){
    char server_msg[256] = "you have reached to the server!";

    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //define the server addr
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    //binding connection
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    //listen for connection
    listen(server_socket, 3);

    int clint_socket;
    clint_socket = accept(server_socket, NULL,NULL);

    send(clint_socket, server_msg, sizeof(server_msg), 0);

    close(server_socket);

    return 0;
}