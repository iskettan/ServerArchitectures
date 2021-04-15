
// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <cerrno>
#include <iostream>
#include <vector>
#include <socketHelper.h>
#include <thread>
#include "spdlog/spdlog.h"

#define PORT 5555
#define _unused(x) [&x]{}()

void serve_client( int client_fd )
{
    while( true ) {
        
        char buf[1024];
        u_int16_t message_size;
        read_message_max_1024( client_fd, buf, message_size );

        if( message_size == 0 ) {
            // Client closed connection.
            break;
        }

        for(int i = 0; i < message_size; i++)
            std::cout << buf[i];
        std::cout << std::endl;

        const char *hello = "Hello from server";
        send( client_fd , hello , strlen(hello) , 0 );
        printf( "Hello message sent\n" );
    }
}

int main(int argc, const char *argv[])
{
    if( argc != 3 ) {
        perror( "Wrong number of arguments passed" );
        exit( EXIT_FAILURE );
    }

    if( strcmp(argv[1], "-c") != 0 ) {
        perror(" -c flag must be included." );
        exit( EXIT_FAILURE );
    }

    const char * configLocation = argv[2];
    _unused( configLocation );

    spdlog::info("Server started.");

    struct sockaddr_in server_address;

    // Creating a server socket.
    int server_fd = server_socket_create( server_address, PORT );
       
    // Attaching socket to the port 8080
    server_socket_bind( server_address, server_fd );

    // Prepare socket for listening
    server_socket_listen( server_fd );

    spdlog::info("Server listening to port {}.", PORT);


    while(true) {
        int client_fd = server_socket_accept( server_address, server_fd );
        // std::thread thread_obj(serve_client, client_fd);
        // thread_obj.detach();
        serve_client( client_fd );
    }

    return 0;
}