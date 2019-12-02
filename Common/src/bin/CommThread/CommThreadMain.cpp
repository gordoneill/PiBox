#include "BluetoothIF.h"
#include "WMsg_encoder.h"
#include "WMsg_decoder.h"
#include "msgQueueConst.h"
#include <string>
#include <iostream>
#include <mqueue.h>
#include <queue>
#include <stdlib.h>
#include <signal.h>

extern int errno;

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

std::queue <WMessage> sendQueue;
mqd_t sendBox, recvBox;

static void sendBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    if(mq_getattr(sendBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Entering MQStat.mq_curmsgs: %ld\n", MQStat.mq_curmsgs);
    
    WMessage msgIn;
    while (errno != EAGAIN)
    {
        mq_receive(sendBox, (char *) &msgIn, 8192 , NULL);
        sendQueue.push(msgIn);
    }
    
    if(mq_getattr(sendBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    
    printf("On Exiting MQStat.mq_curmsgs: %ld\n", MQStat.mq_curmsgs);

    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = sendBoxOnData;
    signal.sigev_notify_attributes = NULL;
    if(mq_notify(sendBox, &signal) == ERROR)
    {
        perror("mq_notify");
        return;
    }

    return;
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
    system("mkdir Logs");
    okay = okay && logger.setLogfile("Logs/CommThread.log");

    //_____________________________________________________________________________

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open(sendQueueName, O_RDWR|O_CREAT|O_EXCL|O_NONBLOCK, QUEUE_PERMISSIONS, attr);
    // mailbox to put messges in received over bluetooth
    recvBox = mq_open(recvQueueName, O_RDWR|O_CREAT|O_EXCL, QUEUE_PERMISSIONS, attr);
    if (sendBox == ERROR)
    {
        mq_unlink(sendQueueName);
        okay = false;
        logger.logEvent(eLevels::FATAL, "sendBox opening failed!");
        std::cerr << "sendBox opening failed!" << std::endl;
    }
    if (recvBox == ERROR)
    {
        mq_unlink(recvQueueName);
        okay = false;
        logger.logEvent(eLevels::FATAL, "recvBox opening failed!");
        std::cerr << "recvBox opening failed!" << std::endl;
    }
    
    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = sendBoxOnData;
    signal.sigev_notify_attributes = NULL;
    signal.sigev_value.sival_ptr = &sendBox;
    if(mq_notify(sendBox, &signal) == ERROR)
    {
        perror("mq_notify");
        okay = false;
    }
    
    //____________________________________________________________________________
    
    Bluetooth connection(logger);

    switch(systemType)
    {
        case CONTROLLER:
            okay = okay && connection.connectToConsole();
            logger.logEvent(eLevels::INFO, "Controller CommThread Starting...");
            std::cout << "Controller CommThread Starting..." << std::endl;
            break;
        case CONSOLE:
            okay = okay && connection.connectToController();
            logger.logEvent(eLevels::INFO, "Console CommThread Starting...");
            std::cout << "Console CommThread Starting..." << std::endl;
            break;
    }
    
    //_____________________________________________________________________________

    WPacket payload;
    WMessage msgIn;
    while(okay)
    {
        if (connection.isDataAvailable()) // if data is coming in over bluetooth
        {
            int bytesRead = 0;
            WPacket payload;
            okay = okay && connection.receive(bytesRead, (char *) &payload);
            WMessage msgIn = RxWMsg(payload);
            logger.logEvent(eLevels::INFO, "received message, type: %d", msgIn.type);
            okay = okay && mq_send(recvBox, (const char *) &msgIn, sizeof(msgIn), 0) == OK;
        }
        if (!sendQueue.empty()) // if data needs to be sent over bluetooth
        {
            msgIn = sendQueue.front();
            WPacket payload = TxWMsg(msgIn);
            logger.logEvent(eLevels::INFO, "sent message");
            sendQueue.pop();
            okay = okay && connection.send(sizeof(payload), (char *) &payload);
        }
    }

    //_____________________________________________________________________________

    logger.logEvent(eLevels::FATAL, "okay became false! Program terminating");
    std::cerr << "okay became false! Program terminating" << std::endl;

    mq_unlink(sendQueueName);
    mq_unlink(recvQueueName);
    mq_close(sendBox);
    mq_close(recvBox);

    return okay;
}
