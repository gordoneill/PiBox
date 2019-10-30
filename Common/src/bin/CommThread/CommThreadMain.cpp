#include "BluetoothIF.h"
#include "WMsg_encoder.h"
#include "WMsg_decoder.h"
#include <string>
#include <iostream>
#include <mqueue.h>
#include <queue>
#include <stdlib.h>
#include <signal.h>

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MQ_MSG_SIZE 256

extern int errno;
int errnum;

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

int main(int argc, char *argv[])
{
    bool okay = true;
    eSystemType systemType = eSystemType::CONSOLE;
    if (argc > 1)
    {
        std::string inputSystem(argv[1]);
        if (inputSystem == "CONTROLLER" || inputSystem == "controller")
        {
            systemType = eSystemType::CONTROLLER;
        }
    }

    LogMgr logger;
    system("mkdir Logs");
    okay = okay && logger.setLogfile("Logs/CommThread.log");
    Bluetooth connection(logger);
    switch(systemType)
    {
        case CONTROLLER:
            //okay = okay && connection.connectToConsole();
            break;
        case CONSOLE:
            //okay = okay && connection.connectToController();
            break;
    }

    mqd_t sendBox, recvBox;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open("/sendBox", O_RDWR|O_CREAT|O_EXCL|O_NONBLOCK, QUEUE_PERMISSIONS, attr);
    // mailbox to put messges in received over bluetooth
    recvBox = mq_open("/recvBox", O_RDWR|O_CREAT|O_EXCL, QUEUE_PERMISSIONS, attr);

    if (sendBox == ERROR)
    {
        mq_unlink("/sendBox");
        okay = false;
        logger.logEvent(eLevels::FATAL, "sendBox opening failed!");
        std::cerr << "sendBox opening failed!" << std::endl;
    }
    if (recvBox == ERROR)
    {
        mq_unlink("/recvBox");
        okay = false;
        logger.logEvent(eLevels::FATAL, "recvBox opening failed!");
        std::cerr << "recvBox opening failed!" << std::endl;
    }

    WPacket payload;
    WMessage msgIn;
    while(okay)
    {
        // if (connection.isDataAvailable()) // if data is coming in over bluetooth
        // {
        //     int bytesRead = 0;
        //     WPacket payload;
        //     okay = okay && connection.receive(bytesRead, (char *) &payload);
        //     WMessage msgIn = RxWMsg(payload);
        //     mq_send(recvBox, (char *) &msgIn, sizeof(msgIn), 1);
        // }
        if (mq_receive(sendBox, (char *) &msgIn, sizeof(msgIn), NULL) > 0)
        {
            WPacket payload = TxWMsg(msgIn);
            std::cout << "received message" << std::endl;
            logger.logEvent(eLevels::INFO, "received message");
            //okay = okay && connection.send(sizeof(payload), (char *) &payload);
        }
    }

    logger.logEvent(eLevels::FATAL, "okay became false! Program terminating");
    std::cerr << "okay became false! Program terminating" << std::endl;

    mq_unlink("/sendBox");
    mq_unlink("/recvBox");
    mq_close(sendBox);
    mq_close(recvBox);

    return okay;
}
