#pragma once
#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <string>

class ConfigParser {
  public:
    void commandLineParser(int, const char *[]);
  private:
    void parseConfig(const char *);
    void setLogLevel( std::string value );
    void setServerPort( std::string value );
    void setServerResponds( std::string value );
    void setMaxMsgSize( std::string value );
    void setThreadPoolSize( std::string value );
    void setProcessPoolSize( std::string value );
};

#endif