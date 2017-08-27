#include "abGameServer.h"
#include <iostream>

int32_t main(int32_t argc, char* argv[])
{
    GameServer gameServer;

    try
    {
        gameServer.initialize(argc, argv);

        if( gameServer.isInitialized() )
            gameServer.spin();
    }
    catch(const std::exception& ex)
    {
        gameServer.printUsage();

        std::stringstream ss; ss << endl;
        ss << "----------------------------------------" << endl
           << "!!!             Exception            !!!" << endl
           << "----------------------------------------" << endl
           << ex.what() << endl;
           PRINT_STACK_TRACE(ss);
        ss << "----------------------------------------" << endl;

        gameServer.logException(ss.str());

        return -1;
    }

    return 0;
}
