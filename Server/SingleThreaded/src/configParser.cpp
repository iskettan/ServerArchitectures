#include "configParser.h"
#include "spdlog/spdlog.h"
#include <string>
#include <limits.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include "config.h"

#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

/**
 * Parse the commandline arguments.
 *
 * @param argc number of arguments
 * @param argv array of arguments
 */
void ConfigParser::commandLineParser(int argc, const char *argv[]) {
/******************************************************************/
    if (argc == 1) {
        spdlog::error("Please, provide a config file!");
        exit( EXIT_FAILURE );
    }
    else if (argc == 2)
        ConfigParser::parseConfig( argv[1] );
    else {
        spdlog::error("Wrong number of arguments has been given");
        exit( EXIT_FAILURE );
    }
}

/**
 * Parse the config file.
 *
 * @param filePath a string to the config file path
 */
void ConfigParser::parseConfig( const char *filePath ) {
/******************************************************/
    if (!fs::exists(filePath)) {
        spdlog::error( "config file path is wrong" );
        exit( EXIT_FAILURE );
    }

    std::ifstream infile( filePath );
    std::string line;

    while ( std::getline( infile, line ) ) {
        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;

        if( key == "LOG_LEVEL" )
            setLogLevel( value );
        else if( key == "SERVER_PORT" )
            setServerPort( value ); 
        else if( key == "SERVER_RESPONDS" )
            setServerResponds( value ); 
        else if( key == "MAX_MSG_SIZE" )
            setMaxMsgSize( value );
        else if( key == "THREADPOOL_SIZE" )
            setThreadPoolSize( value );
        else if( key == "PROCESSPOOL_SIZE" )
            setProcessPoolSize( value );
    }
}
void ConfigParser::setLogLevel( std::string value )
/*************************************************/
{
    spdlog::set_pattern("[%H:%M:S:%f] [%^%l%$] [Th: %t] %v");

    if (value == "debug")
        spdlog::set_level(spdlog::level::debug);
    else if (value == "info")
        spdlog::set_level(spdlog::level::info);
    else if (value == "error")
        spdlog::set_level(spdlog::level::err);
    else if (value == "critical")
        spdlog::set_level(spdlog::level::critical);
    else if (value == "off")
        spdlog::set_level(spdlog::level::off);
    else {
        spdlog::error("wrong LOG_LEVEL in config file");
        exit( EXIT_FAILURE );
    }
    spdlog::info( "LOG_LEVEL has been set to: {}", value );
}

void ConfigParser::setServerPort( std::string value )
/***************************************************/
{
    CONFIG::SERVER_PORT = (u_int16_t) strtoul( value.c_str(), NULL, 0 );
    if( CONFIG::SERVER_PORT == 0 ) {
        spdlog::error( "wrong PORT_BUMBER in config file" );
        exit( EXIT_FAILURE );
    }
    spdlog::info( "Server PORT_NUMBER has been set to: {}", CONFIG::SERVER_PORT );
}

void ConfigParser::setServerResponds( std::string value )
/*******************************************************/
{
    if( value == "true" ) {
        CONFIG::SERVER_RESPONDS = true;
    } else {
        CONFIG::SERVER_RESPONDS = false;
    }
    spdlog::info( "SERVER_RESPONDSs has been set to: {}", CONFIG::SERVER_RESPONDS );

}

void ConfigParser::setMaxMsgSize( std::string value )
/***************************************************/
{
    CONFIG::MAX_MSG_SIZE = (u_int16_t) strtoul( value.c_str(), NULL, 0 );
    if( CONFIG::MAX_MSG_SIZE == 0 ) {
        spdlog::error( "wrong MAX_MSG_SIZE in config file" );
        exit( EXIT_FAILURE );
    }
    spdlog::info( "Server PORT_NUMBER has been set to: {}", CONFIG::MAX_MSG_SIZE );
}

void ConfigParser::setThreadPoolSize( std::string value )
/***************************************************/
{
    CONFIG::THREADPOOL_SIZE = (u_int16_t) strtoul( value.c_str(), NULL, 0 );
    if( CONFIG::THREADPOOL_SIZE == 0 ) {
        spdlog::error( "wrong THREADPOOL_SIZE in config file" );
        exit( EXIT_FAILURE );
    }
    spdlog::info( "Server THREADPOOL_SIZE has been set to: {}", CONFIG::THREADPOOL_SIZE );
}

void ConfigParser::setProcessPoolSize( std::string value )
/***************************************************/
{
    CONFIG::PROCESSPOOL_SIZE = (u_int16_t) strtoul( value.c_str(), NULL, 0 );
    if( CONFIG::PROCESSPOOL_SIZE == 0 ) {
        spdlog::error( "wrong PROCESSPOOL_SIZE in config file" );
        exit( EXIT_FAILURE );
    }
    spdlog::info( "Server PROCESSPOOL_SIZE has been set to: {}", CONFIG::PROCESSPOOL_SIZE );
}