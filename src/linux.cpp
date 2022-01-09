#include "linux.hpp"

size_t str_len(const char *arr)
{
    if(!arr)
        return 0;
    size_t size = 0;
    while (arr[size])
    {
        size++;
    }
    return size;
}

std::vector<unsigned long> get_pids_list()
{
    std::vector<unsigned long> pids;
    DIR *dir;

    if (!(dir = opendir("/proc")))
        exit(1);

    while (dirent *dirp = readdir(dir))
    {
        if (dirp->d_type != DT_DIR)
            continue;

        if (!std::all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c)
                         { return std::isdigit(c); }))
            continue;

        pids.push_back(std::stoi(dirp->d_name));
    }
    closedir(dir);
    return pids;
}

const char *get_process_name_by_pid(const unsigned long pid)
{
    const int PID_NAME_LEN = 32;
    char *location = (char *)malloc(PID_NAME_LEN * sizeof(char));
    sprintf(location, "/proc/%lu/status", pid);

    FILE *pid_file = fopen(location, "r");

    if (!pid_file)
    {
        exit(2);
    }
    free(location);

    char *pid_name = (char *)malloc(PID_NAME_LEN * sizeof(char));
    fgets(pid_name, PID_NAME_LEN, pid_file);

    const int NAME_SHIFT = 6, NAME_END = 20;
    memcpy(pid_name, pid_name + (NAME_SHIFT * sizeof(char)), NAME_END);
    pid_name[(str_len(pid_name) - 1)] = '\0';

    return pid_name;
}

int kill_proc_by_pid(unsigned long pid){

    return kill( (pid_t)pid, 9);
}

std::vector<Proc_Info> get_pid_and_name()
{
    const int MAX_PATH_LEN = 260;
    std::vector<Proc_Info> pi;
    std::vector<unsigned long> pids = get_pids_list();
    for(auto i: pids)
    {

        Proc_Info temp;
        temp.pid = i;
        temp.name = (char*)malloc(MAX_PATH_LEN);
        strcpy(temp.name, get_process_name_by_pid(i));
        pi.push_back(temp);
    }
    return pi;
}