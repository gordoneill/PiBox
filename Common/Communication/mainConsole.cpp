#include "bluetoothIF.h"
#include <string>

int main(int argc, char *argv[])
{
    bool okay = true;
    LogMgr logger;
    okay = okay && logger.setLogfile("CommunicationLog.log");
    Bluetooth connection(logger);
    okay = okay && connection.connectToController();
    
    int bytesRead = 0;
    char data[16];
    okay = okay && connection.receive(bytesRead, (char *) &data);
    
    std::string dataRx(data);
    okay = okay && logger.logEvent(eLevels::INFO, dataRx.c_str());
    
    return okay;
}
