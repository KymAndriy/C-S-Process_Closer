#include "windows.hpp"

std::vector<Proc_Info> get_pid_and_name()
{
    std::vector<Proc_Info> p_n;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot) {
        PROCESSENTRY32 pe32;

        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32)) {
            do {
                Proc_Info pi;
                pi.name = (char*)malloc(MAX_PATH);
                pi.pid = pe32.th32ProcessID;
                strcpy(pi.name, pe32.szExeFile);
                p_n.push_back(pi);
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return p_n;
}

int kill_proc_by_pid(unsigned long pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE,
        FALSE, pid);
    if (hProcess == NULL)
    {
        return -3;
    }

    if (!TerminateProcess(hProcess, 0))
    {
        return -4;
    }
    CloseHandle(hProcess);
    return 0;
}

