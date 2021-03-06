#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>

int main(){

    // create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an addr for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002); //portNum
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // printf("%d", connection_status);
    // check for errors with the connection!
    if (connection_status == -1){
        printf("Problem in connecting with the socket\n\n");
    }

    // recieve data from the server
    char server_response[256];
    recv(network_socket , &server_response, sizeof(server_response), 0);

    // printing the recieve data as response!
    printf("Data from server: %s\n",server_response);

    close(network_socket);
    return 0;
}