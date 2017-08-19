#include <stdexcept>
#include <sstream>
#include <iostream>

#include "DerivedClass.h"
#include "buildVersion.h"

#if STACK_TRACING_ENABLED
    #define PRINT_STACK_TRACE(os)           \
        os << "--------- Stack---------\n"  \
           << Stack::GetTraceString() << std::endl;
#else
    #define PRINT_STACK_TRACE(os)
#endif

int main(int argc, char* argv[])
{
    UNUSED_PARAM(argc)
    UNUSED_PARAM(argv)

    try
    {
        DerivedClass derived;

        derived.print();

    }
    catch (const std::exception& ex)
    {
        std::cout << std::endl;

        std::stringstream ss; ss << std::endl;
        ss << "----------------------------------------" << std::endl
           << "!!!             Exception            !!!" << std::endl
           << "----------------------------------------" << std::endl
           << ex.what() << std::endl;
           PRINT_STACK_TRACE(ss);
        ss << "----------------------------------------" << std::endl;
    }



    return 0;
}
