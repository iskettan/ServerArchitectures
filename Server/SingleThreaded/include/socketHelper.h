#pragma once

#ifndef _SOCKETHELPER_H_
#define _SOCKETHELPER_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <cerrno>
#include <iostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#define NUMBER_OF_CONNECTIONS_QUEUED_UP 128

void set_non_blocking_socket( int fd )
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl failed (F_GETFL)");
        exit(EXIT_FAILURE);
    }

    flags |= O_NONBLOCK;
    int s = fcntl(fd, F_SETFL, flags);
    if (s == -1)
    {
        perror("fcntl failed (F_SETFL)");
        exit(EXIT_FAILURE);
    }
}

// WILL BE PROBLEMATIC
int accept_connection( int socketfd, struct epoll_event& event, int epollfd, struct sockaddr_in &out_addr )
{
    socklen_t in_len = sizeof(out_addr);
    int infd = accept(socketfd, ( struct sockaddr * )&out_addr, &in_len);
    if (infd == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // Done processing incoming connections
        {
            return -1;
        }
        else
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
    }

    set_non_blocking_socket( infd );

    event.data.fd = infd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, infd, &event) == -1)
    {
        perror("epoll_ctl failed");
        exit(EXIT_FAILURE);
    }

    return infd;
}

int epoll_create()
/****************/
{
    int epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }
    return epollfd;
}

int server_socket_create(struct sockaddr_in &server_address, u_int16_t port)
/**************************************************************************/
{
    int server_fd;

    // Creating socket file descriptor
    if( ( server_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == 0 ) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port );

    return server_fd;
}

void server_socket_bind( struct sockaddr_in &server_address, int server_fd )
/**************************************************************************/
{
    if( bind( server_fd, ( struct sockaddr * )&server_address, sizeof( server_address ) ) <0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void server_socket_listen( int server_fd )
/****************************************/
{
    if( listen( server_fd, NUMBER_OF_CONNECTIONS_QUEUED_UP ) < 0 )
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int server_socket_accept( int server_fd, struct sockaddr_in &client_address )
/***************************************************************************/
{
    int client_socket;
    int addrlen = sizeof(client_address);

    if( ( client_socket = accept( server_fd, ( struct sockaddr * )&client_address, ( socklen_t * )&addrlen) ) < 0 )
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return client_socket;
}

/// Reads n bytes from fd.
bool read_nBytes( int fd, void *buf, std::size_t n )
/**************************************************/
{
    std::size_t offset = 0;
    char *cbuf = reinterpret_cast<char*>( buf );
    while ( true ) {
        ssize_t ret = recv( fd, cbuf + offset, n - offset, MSG_WAITALL );
        if ( ret < 0 ) {
            if ( errno != EINTR ) {
                // Error occurred
                return false;             
            }
        } else if ( ret == 0 ) {
            // No data available anymore
            if ( offset == 0 )
                return false;
            else {
                perror( "Unexpected end of stream" );
                exit( EXIT_FAILURE );    
            }
        } else if ( offset + ret == n ) {
            // All n bytes read
            return true;
        } else {
            offset += ret;
        }
    }
}

/// Reads n bytes from fd.
bool send_nBytes( int fd, const void *buf, std::size_t n )
/**************************************************/
{
    std::size_t offset = 0;
    const char *cbuf = reinterpret_cast<const char*>( buf );
    while ( true ) {
        ssize_t ret = send( fd, cbuf + offset, n - offset, MSG_NOSIGNAL );
        if ( ret < 0 ) {
            if ( errno != EINTR ) {
                return false;      
            }
        } else if ( ret == 0 ) {
            // No data available anymore
            if ( offset == 0 )
                return false;
            else {
                perror( "Unexpected end of stream" );
                exit( EXIT_FAILURE );    
            }
        } else if ( offset + ret == n ) {
            // All n bytes read
            return true;
        } else {
            offset += ret;
        }
    }
}

/// Reads message from fd
void read_message_max_1024( int fd, char *buf, u_int16_t &buf_size )
/******************************************************************/
{
    if ( read_nBytes( fd, &buf_size, sizeof( buf_size ) ) ) {

        if( buf_size > 1024 ) {\
            // If client sends a message with more than expected size
            buf_size = 0;
            close( fd );
            return;
        }

        if( read_nBytes( fd, buf, buf_size ) ) {
            return;
        } else {
            buf_size = 0;
            return;
        }

    } else {
        // connection was closed
        buf_size = 0;
        return;
    }
}

/// Reads message from fd
void send_message( int fd, const char *buf, u_int16_t &buf_size )
/******************************************************************/
{
    if( send_nBytes( fd, &buf_size, sizeof( buf_size ) ) ) {
        if( !send_nBytes( fd, buf, buf_size ) ) {
            // connection was closed
            buf_size = 0;
            return;  
        }
    } else {
        // connection was closed
        buf_size = 0;
        return;
    }
}

#endif