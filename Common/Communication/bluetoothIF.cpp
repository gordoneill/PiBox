#include <stdio.h>
#include <unistd.h>
#include "bluetoothIF.h"

Bluetooth::Bluetooth(LogMgr & logger) :
    connected_(false),
    client_(-1),
    logger_(logger)
{
    socket_ = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
}

Bluetooth::~Bluetooth()
{
    close(client_);
    close(socket_);
}

bool Bluetooth::connectToController()
{
    struct sockaddr_rc loc_addr = { 0 };
    struct sockaddr_rc rem_addr = { 0 };
    bdaddr_t bdaddrAny = {{0,0,0,0,0,0}};
    
    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = bdaddrAny;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(socket_, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    
    listen(socket_, 1);

    logger_.logEvent(eLevels::INFO, 
            "Bluetooth::connectToController - waiting for connection");

    // accept one connection
    socklen_t opt = sizeof(rem_addr);
    client_ = accept(socket_, (struct sockaddr *)&rem_addr, &opt);

    connected_ = true;
    
    logger_.logEvent(eLevels::INFO, 
            "Bluetooth::connectToController - socket_: %d, client_: %d", 
            socket_, client_);

    return connected_;
}

bool Bluetooth::connectToConsole()
{
    struct sockaddr_rc addr = { 0 };
    
    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(CONSOLE_ADDR, &addr.rc_bdaddr);

    // connect to server
    while (!connected_)
    {
        logger_.logEvent(eLevels::WARNING, 
                "Bluetooth::connectToConsole - attemping connection ...");
        connected_ = connect(socket_, (struct sockaddr *)&addr, sizeof(addr)) == OK;
        sleep(5);
    }

    client_ = socket_;
    
    logger_.logEvent(eLevels::INFO, 
                "Bluetooth::connectToConsole - connected: %d", connected_);

    return connected_;
}

bool Bluetooth::send(int size, char * data)
{
    bool okay = true;
    if (connected_)
    {
        okay = okay && write(client_, data, size) == size;
        if(!okay)
        {
            logger_.logEvent(eLevels::FATAL, 
                "Bluetooth::send - failed to write");
        }
    }
    else
    {
        logger_.logEvent(eLevels::FATAL, 
            "Bluetooth::send - not connected");
    }
    return okay;
}

bool Bluetooth::receive(int & bytesRead, char * data)
{
    bool okay = true;
    if (connected_)
    {
        int dataAmount = 0;
        ioctl(client_, SIOCINQ, &dataAmount);
        // read data from the client
        bytesRead = read(client_, data, dataAmount);
        if(bytesRead > 0) 
        {
            printf("received [%s]\n", data);
        }
    }
    else
    {
        okay = false;
        logger_.logEvent(eLevels::FATAL, 
            "Bluetooth::receive - not connected");
    }
    return okay;
}

bool Bluetooth::isDataAvailable()
{
    bool data = false;
    int dataAmount = 0;
    ioctl(client_, SIOCINQ, &dataAmount);
    if (dataAmount > 0)
    {
        data = true;
    }
    return data;
}
