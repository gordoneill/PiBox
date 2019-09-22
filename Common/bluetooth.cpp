#include <stdio.h>
#include <unistd.h>

void bluetooth::bluetooth(logMgr logger) :
    connected_(false),
    client_(-1),
    logger_(logger)
{
    socket_ = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
}

void bluetooth::~bluetooth()
{
    close(client);
    close(s);
}

bool bluetooth::connectToController()
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    
    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // accept one connection
    socklen_t opt = sizeof(rem_addr);
    client_ = accept(socket_, (struct sockaddr *)&rem_addr, &opt);

    listen(socket_, 1);

    connected_ = true;

    return connected_;
}

bool bluetooth::connectToConsole()
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    
    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(CONSOLE_ADDR, &addr.rc_bdaddr);

    // connect to server
    connected_ = connect(socket_, (struct sockaddr *)&addr, sizeof(addr));

    return connected_;
}

bool bluetooth::send(int size, byte & data)
{
    if (connected_)
    {
        bool okay = true;
        okay = write(socket_, data, size);
        if(!okay)
        {

        }
    }
    else
    {
        
    }
}

bool bluetooth::receive(int & bytesRead, byte & data)
{
    if (connected_)
    {
        memset(data, 0, sizeof(data));

        // read data from the client
        bytesRead = read(client_, data, sizeof(data));
        if(bytesRead > 0) 
        {
            printf("received [%s]\n", data);
        }
    }
    else
    {

    }
}