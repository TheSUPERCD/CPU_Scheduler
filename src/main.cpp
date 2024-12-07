#include"./headers/plist.hpp"
#include <cstring>


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

    int elements_to_display = 20;
    if(argc>2){
        elements_to_display = strcmp(argv[2], "--display-all")==0 ? -1 : elements_to_display;
    }

    std::cout<<"============================= FIFO Scheduler ============================="<<std::endl;
    plist.display(FIFO, elements_to_display);
    std::cout<<"=================== SJF Scheduler (Without Preemption) ==================="<<std::endl;
    plist.display(SJF, elements_to_display);
    std::cout<<"================== PRIORITY Scheduler (With Preemption) =================="<<std::endl;
    plist.display(PRIO, elements_to_display);

    

    return 0;
}
