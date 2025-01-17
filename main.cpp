#include "args.h"



int main(int argc, char *argv[])
{
    if(!args::check(argc, argv))
    {
        return 1;
    }
    std::cout << "Passed";
    
    
    return 0;
   
}