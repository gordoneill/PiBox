#include "BluetoothIF.h"
#include "WMsg_encoder.h"
#include "WMsg_decoder.h"
#include <string>
#include <iostream>
#include <mqueue.h>
#include <queue>
#include <stdlib.h>
#include <signal.h>

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

std::queue<WMessage> sendQueue;

static void sendBoxOnData(union sigval sv)
{
    std::cout << "something is sending!" << std::endl;
    struct mq_attr attr;
    ssize_t nr;
    WMessage payloadIn;
    mqd_t sendBox = *((mqd_t *) sv.sival_ptr);
    mq_receive(sendBox, (char *) &payloadIn, sizeof(payloadIn), NULL);
    sendQueue.push(payloadIn);
}

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
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open("sendBox", O_RDONLY|O_CREAT|O_EXCL, 0666, 0);
    // mailbox to put messges in received over bluetooth
    recvBox = mq_open("recvBox", O_RDWR|O_CREAT|O_EXCL, 0666, 0);

    if (sendBox == ERROR || recvBox == ERROR)
    {
        okay = false;
        logger.logEvent(eLevels::FATAL, "sendBox or recvBox opening failed!");
        std::cerr << "sendBox or recvBox opening failed!" << std::endl;
    }

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = sendBoxOnData;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &sendBox;
    mq_notify(sendBox, &sev);

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
        if (!sendQueue.empty()) // if data needs to be sent over bluetooth
        {
            WPacket payload = TxWMsg(sendQueue.front());
            //okay = okay && connection.send(sizeof(payload), (char *) &payload);
            std::cout << "send loop!" << std::endl;
            if (okay)
            {
                sendQueue.pop();
            }
        }
    }

    logger.logEvent(eLevels::FATAL, "okay became false! Program terminating");
    std::cerr << "okay became false! Program terminating" << std::endl;

    mq_unlink("sendBox");
    mq_unlink("recvBox");
    mq_close(sendBox);
    mq_close(recvBox);

    return okay;
}
