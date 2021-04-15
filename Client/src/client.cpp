// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define PORT 5555
#define _unused(x) [&x]{}()

int main(int argc, char const *argv[])
{
    _unused( argc );
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    std::string hello = "Hello from client #";
    hello += argv[1];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(true) {
        uint16_t size = hello.size();
        send(sock , &size, sizeof(size) , 0 );
        send(sock , hello.c_str() ,hello.size() , 0 );
        printf("Hello message sent\n");

        char buffer[1024] = {0};
        ssize_t valread = read( sock , buffer, 1024);
        _unused(valread);
        std::cout << "RECIEVED: " << buffer << std::endl;

        sleep(1);
    }

    // uint16_t size = hello.size();
    // send(sock , &size, sizeof(size) , 0 );
    // send(sock , hello.c_str() ,hello.size() , 0 );
    // printf("Hello message sent\n");

    // char buffer[1024] = {0};
    // valread = read( sock , buffer, 1024);
    // std::cout << "RECIEVED: " << buffer << std::endl;
    return 0;
}