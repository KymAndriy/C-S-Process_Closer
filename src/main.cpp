#include "main.hpp"

//#include "windows.hpp"


int main()
{
    std::vector<Proc_Info> pid_name = get_pid_and_name();

    for(auto i: pid_name){
        std::cout << i.name << " - " << i.pid << std::endl;
    }
    unsigned long cl = 0;
    std::cin >> cl;

    auto it = std::find_if(pid_name.begin(), pid_name.end(), [&cl](const Proc_Info& obj) {return obj.pid == cl; });

    std::cout << it->name << "\t - " << it->pid <<  "\tclose code - " << kill_proc_by_pid(cl) << std::endl;
    system("pause");
}