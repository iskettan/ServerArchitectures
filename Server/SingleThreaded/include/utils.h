#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

#include "spdlog/spdlog.h"

#define CALCULATE_THROUGHPUT_EVERY_X_OPERATIONS 10000

std::atomic<u_int32_t> operations_count = 0;
std::atomic<u_int32_t> current_active_connections = 0;
std::mutex mx;
auto t1 = std::chrono::high_resolution_clock::now();

inline void calculate_server_performance()
/****************************************/
{
    int current_operations_count = operations_count++;
    current_operations_count++;
    if( current_operations_count % CALCULATE_THROUGHPUT_EVERY_X_OPERATIONS == 0 ) {
        mx.lock();
        
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
        double throughput = CALCULATE_THROUGHPUT_EVERY_X_OPERATIONS / ( duration / 1000.0 );

        spdlog::info( "*************************************************************" );
        spdlog::info( "Duration to increment {} oparations is : {} ms.", CALCULATE_THROUGHPUT_EVERY_X_OPERATIONS, duration );
        spdlog::info( "Throughput is : {} OPS.", throughput );
        spdlog::info( "Current active connections is : {}.", current_active_connections );
        spdlog::info( "*************************************************************" );

        t1 = t2;
        mx.unlock();
    }
}

inline bool handle_client_request( int client_fd, struct sockaddr_in &client_address, char *message, uint16_t message_length )
/**********************************************************************************************************************************/
{
    spdlog::info( " handling client request" );
    _unused( client_address );
    _unused( message );
    _unused( message_length );
    #ifdef DEBUG
        message[message_length] = 0;
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &( client_address.sin_addr ), client_ip, sizeof( client_address ) );
        spdlog::debug( "Received from client {}:{} this message: {}", client_ip, ntohs( client_address.sin_port ), message );
    #endif
    
    // PerformYourOperation();

    // If SERVER_RESPONDS option is set, the server will respond back to the client.
    if( CONFIG::SERVER_RESPONDS ) {
        const char *hello = "Hello from server";
        uint16_t message_size = strlen( hello );
        send_message( client_fd , hello ,message_size );
        if( message_size == 0 ) {
            // Client closed connection.
            return false;
        }
        #ifdef DEBUG
        spdlog::debug( "Sent to client {}:{} this message: {}", client_ip, ntohs( client_address.sin_port ), hello );
        #endif
    }

    calculate_server_performance();
    return true;

}

#endif