#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <cerrno>
#include <iostream>

#define NUMBER_OF_CONNECTIONS_QUEUED_UP 32

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

int server_socket_accept( struct sockaddr_in &server_address, int server_fd )
/***************************************************************************/
{
    int client_socket;
    int addrlen = sizeof(server_address);

    if( ( client_socket = accept( server_fd, ( struct sockaddr * )&server_address, ( socklen_t * )&addrlen) ) < 0 )
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
                perror( "Error occured while receiving" );
                exit( EXIT_FAILURE );            
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
        std::cout << buf_size << std::endl;

        if( buf_size > 1024 ) {\
            // If client sends a message with more than expected size
            buf_size = 0;
            close( fd );
            return;
        }

        if( read_nBytes( fd, buf, buf_size ) ) {
            return;
        } else {
            perror( "Unexpected end of stream" );
            exit( EXIT_FAILURE );
        }

    } else {
        // connection was closed
        buf_size = 0;
        return;
    }
}