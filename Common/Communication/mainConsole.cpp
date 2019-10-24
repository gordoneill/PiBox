#include "bluetoothIF.h"
#include <string>

int main(int argc, char *argv[])
{
    bool okay = true;
    LogMgr logger;
    okay = okay && logger.setLogfile("CommunicationLog.log") == OK;
    Bluetooth connection(logger);
    okay = okay && connection.connectToController();
    
    int bytesRead = 0;
    std::string dataOut;

    while(1)
    {
        std::cout << "Type something in (followed by enter): \n";
        std::cin >> dataOut;
        okay = okay && connection.send(dataOut.length(), (char *) &dataOut);
        if (connection.isDataAvailable())
        {
            okay = okay && connection.receive(bytesRead, (char *) &dataOut);
            std::cout << dataOut << std::endl;
        }
    }
    
    return okay;
}
