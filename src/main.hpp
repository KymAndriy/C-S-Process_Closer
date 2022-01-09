#ifndef GL_MAIN_HPP
#define GL_MAIN_HPP

#ifdef __linux__
#include "linux.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#elif  _WIN32
#include "windows.hpp"

#endif

union
{
    char buf[sizeof(unsigned long)];
    unsigned long len;
} data_len;

#endif

