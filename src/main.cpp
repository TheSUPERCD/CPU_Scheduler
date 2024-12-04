#include"./headers/plist.hpp"


#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define NORMAL "\033[0m"
#define RED "\033[1;31m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"

#define ERROR(err_str) {std::cerr<<RED<<"\nERROR: "<< err_str <<NORMAL<<std::endl; exit(EXIT_FAILURE);}

int main(int argc, char **argv){
    if(argc<2)
        ERROR("Filename not given!")
    std::string filename = argv[1];
    std::ifstream datafile(filename);
    if(!datafile.is_open())
        ERROR("Couldn't open datafile!")
    
    ProcessList plist(datafile);
    plist.FIFO_scheduler();
    plist.SJF_scheduler();
    plist.PRIORITY_scheduler();

    std::cout<<"============================= FIFO Scheduler ============================="<<std::endl;
    plist.display(FIFO, 20);
    std::cout<<"=================== SJF Scheduler (Without Preemption) ==================="<<std::endl;
    plist.display(SJF, 20);
    std::cout<<"================== PRIORITY Scheduler (With Preemption) =================="<<std::endl;
    plist.display(PRIO, 20);

    

    return 0;
}
