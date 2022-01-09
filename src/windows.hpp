#ifndef GL_WINDOWS_HPP
#define GL_WINDOWS_HPP

#include <windows.h>
#include <psapi.h>
#include <Tlhelp32.h>

#include "indentic_inc.hpp"

int kill_proc_by_pid(unsigned long pid);
std::vector<Proc_Info> get_pid_and_name();

#endif

