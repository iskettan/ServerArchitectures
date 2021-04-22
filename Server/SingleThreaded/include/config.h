#pragma once
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

#define _unused(x) [&x]{}()

class CONFIG {
  public:
    static std::string LOG_LEVEL;
    static std::string SERVER_IP;
    static u_int16_t SERVER_PORT;
    static bool SERVER_RESPONDS;
    static u_int16_t MAX_MSG_SIZE;
    static u_int16_t THREADPOOL_SIZE;
    static u_int16_t PROCESSPOOL_SIZE;
};
#endif