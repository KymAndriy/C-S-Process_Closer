#ifndef GL_LINUX_HPP
#define GL_LINUX_HPP

#include <proc_service.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include "indentic_inc.hpp"


int kill_proc_by_pid(unsigned long pid);
std::vector<Proc_Info> get_pid_and_name();

#endif
