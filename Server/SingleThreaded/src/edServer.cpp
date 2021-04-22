// event driven server.
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
#include <unordered_map>
#include "clientConnection.h"


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

void accept_client( int server_fd )
{
    while(true) {
        struct sockaddr_in client_address;
        int client_fd = server_socket_accept( server_fd, client_address );
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &( client_address.sin_addr ), client_ip, sizeof( client_address ) );
        spdlog::debug( "Client {}:{} has connected to the server.", client_ip, ntohs( client_address.sin_port ) );
        serve_client( client_fd, client_address );
    }
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

    // Set socket as non-blocking
    set_non_blocking_socket( server_fd );

    // Prepare socket for listening
    server_socket_listen( server_fd );
    spdlog::info( "Server listening to port {}.", CONFIG::SERVER_PORT );

    // Create epoll fd
    int epoll_fd = epoll_create();

    // Add the server socket to the epoll_fd
    struct epoll_event event;
    event.data.fd = server_fd;
    event.events = EPOLLIN | EPOLLET;
    if ( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, server_fd, &event ) == -1 )
    if ( epoll_fd == -1 )
    {
        perror("epoll_ctl failed");
        exit(EXIT_FAILURE);
    }

    const int MAX_EVENTS = 128;
    std::array<struct epoll_event, MAX_EVENTS> events;

    t1 = std::chrono::high_resolution_clock::now();

    while (true)
    {
        auto n = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        for (int i = 0; i < n; ++i)
        {
            if (events[i].events & EPOLLERR ||
                events[i].events & EPOLLHUP ||
                !(events[i].events & EPOLLIN)) // error
            {
                std::cerr << "[E] epoll event error\n";
                close(events[i].data.fd);
            }
            else if (server_fd == events[i].data.fd) // new connection
            {
                struct sockaddr_in client_addr;
                int client_fd = -1;
                while ( ( client_fd = accept_connection(server_fd, event, epoll_fd, client_addr) ) == -1 );
                clientConnection *cc = new clientConnection( client_fd, client_addr );
                clientsConnections[client_fd] = cc;

                std::cout << "client fd added: " << client_fd << std::endl;

                char client_ip[INET_ADDRSTRLEN];
                inet_ntop( AF_INET, &( client_addr.sin_addr ), client_ip, sizeof( client_addr ) );
                spdlog::debug( "Client {}:{} has connected to the server.", client_ip, ntohs( client_addr.sin_port ) );
            }
            else // data to read
            {
                auto client_fd = events[i].data.fd;
                std::cout << "client fd sent data on fd: " << client_fd << std::endl;

                clientConnection *cc = clientsConnections[client_fd];
                while (cc->read_data());
            }
        }
    }

    spdlog::critical("should never reach here");
    return 0;
}