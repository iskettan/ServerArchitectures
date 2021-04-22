#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <cstring>
#include <netdb.h>
#include <array>
#include <utils.h>

class clientConnection;

std::unordered_map<int, clientConnection *> clientsConnections;

struct clientConnection {

    struct sockaddr_in _client_address;
    int _client_fd;
    char _buf[1024];
    u_int16_t _remainingBytes;
    u_int16_t _currentReadIndex;
    bool _knowsRemainingBytes;
    u_int16_t _currentRemainingBytesIndex;
    u_int16_t _remainingBytesRemainingBytes;

    clientConnection( int client_fd, struct sockaddr_in client_address )
    {
        _client_address = client_address;
        _client_fd = client_fd;
        _remainingBytes = 0;
        _currentReadIndex = 0;
        _knowsRemainingBytes = false;
        _currentRemainingBytesIndex = 0;
        _remainingBytesRemainingBytes = sizeof( u_int16_t );
    }

    bool read_data()
    {
        if( _knowsRemainingBytes ){

            auto count = read( _client_fd, _buf + _currentReadIndex, _remainingBytes );

            if (count == -1)
            {
                if (errno == EAGAIN) // read all data
                {
                    return false;
                }
            }
            else if (count == 0) // EOF - remote closed connection
            {
                std::cout << "[I] Close " << _client_fd << "\n";
                // close(_client_fd);
                // delete clientsConnections[_client_fd];
                // clientsConnections.erase(_client_fd);
                return false;
            }

            _remainingBytes -= count;
            _currentReadIndex += count;

            handle_client_request( _client_fd, _client_address, _buf, _currentReadIndex );

            if( _remainingBytes == 0 ) {
                _knowsRemainingBytes = false;
                _currentReadIndex = 0;
            }

        } else {
            // we need to determine the # of bytes to read.
            auto count = read( _client_fd, (&_remainingBytes) + _currentRemainingBytesIndex, _remainingBytesRemainingBytes );

            if (count == -1)
            {
                if (errno == EAGAIN) // read all data
                {
                    return false;
                }
            }
            else if (count == 0) // EOF - remote closed connection
            {
                std::cout << "[I] Close " << _client_fd << "\n";
                std::cout << "remaining bytes = " << _remainingBytes << std::endl;
                // close(_client_fd);
                return false;
            }

            _remainingBytesRemainingBytes -= count;
            _currentRemainingBytesIndex += count;

            if( _currentRemainingBytesIndex == 0 ) {
                _knowsRemainingBytes = true;
                _currentRemainingBytesIndex = 0;
                _remainingBytesRemainingBytes = sizeof( u_int16_t );
            }
        }

        return true;
    }

};
