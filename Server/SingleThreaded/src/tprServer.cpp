// Thread per request server.
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
#include "configParser.h"
#include <arpa/inet.h>
#include "config.h"
#include "utils.h"

void serve_client( int client_fd, struct sockaddr_in client_address )
/*******************************************************************/
{
    current_active_connections++;
    while( true ) {
        char buf[1024];
        u_int16_t message_size;
        read_message_max_1024( client_fd, buf, message_size );

        if( message_size == 0 ) {
            // Client closed connection.
            break;
        }

        if( !handle_client_request( client_fd, client_address, buf, message_size ) ) {
            break;
        }
    }
    current_active_connections--;
}

int main(int argc, const char *argv[])
{
/************************************/
    ConfigParser cp;
    cp.commandLineParser( argc, argv );
    spdlog::info( "Server started." );
    
    struct sockaddr_in server_address;

    // Creating a server socket.
    int server_fd = server_socket_create( server_address, CONFIG::SERVER_PORT );
       
    // Attaching socket to the port
    server_socket_bind( server_address, server_fd );

    // Prepare socket for listening
    server_socket_listen( server_fd );

    spdlog::info( "Server listening to port {}.", CONFIG::SERVER_PORT );

    t1 = std::chrono::high_resolution_clock::now();

    while(true) {
        struct sockaddr_in client_address;
        int client_fd = server_socket_accept( server_fd, client_address );
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &( client_address.sin_addr ), client_ip, sizeof( client_address ) );
        spdlog::debug( "Client {}:{} has connected to the server.", client_ip, ntohs( client_address.sin_port ) );
        
        std::thread thread_obj( serve_client, client_fd, client_address );
        thread_obj.detach();
    }

    spdlog::critical("should never reach here");
    return 0;
}