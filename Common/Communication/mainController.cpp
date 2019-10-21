#include "bluetoothIF.h"
#include <string>

int main(int argc, char *argv[])
{
    bool okay = true;
    LogMgr logger;
    okay = okay && logger.setLogfile("CommunicationLog.log");
    Bluetooth connection(logger);
    okay = okay && connection.connectToController();
    
    std::string toSend = "hello";
    okay = okay && connection.send(toSend.length(), (char *)toSend.c_str());
    
    //msg
    //okay = okay && connection.send(sizeof(msg), (char *)&msg);

    return okay;
}
