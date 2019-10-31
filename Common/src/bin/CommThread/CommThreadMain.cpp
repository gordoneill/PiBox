#include "BluetoothIF.h"
#include "WMsg_encoder.h"
#include "WMsg_decoder.h"
#include <string>
#include <iostream>
#include <mqueue.h>
#include <queue>
#include <stdlib.h>
#include <signal.h>

char sendQueueName[10] = "/sendBox";
char recvQueueName[10] = "/recvBox";

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MQ_MSG_SIZE 256

extern int errno;
int errnum;

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

std::queue <WMessage> sendQueue;
mqd_t sendBox, recvBox; 
char Message[100];
struct sigevent SIGNAL;

static void sendBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    WMessage msg;
    if(mq_getattr(sendBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Entering MQStat.mq_curmsgs: %ld\n",MQStat.mq_curmsgs);
    ssize_t NoOfBytesRx = mq_receive(sendBox, Message, MQStat.mq_msgsize, 0);
    
    if(NoOfBytesRx == ERROR)
    {
        perror("mq_receive");
        return;
    }

    //reinterpret_cast<WMessage *>(WMessage * )
    msg.type = (uint32_t) Message[4];
    sendQueue.push(msg);
        
    if(mq_getattr(sendBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Exiting MQStat.mq_curmsgs: %ld\n",MQStat.mq_curmsgs);
    if(mq_notify(sendBox, &SIGNAL) == ERROR)
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

    SIGNAL.sigev_notify = SIGEV_THREAD;
    SIGNAL.sigev_notify_function = sendBoxOnData;
    SIGNAL.sigev_notify_attributes = NULL;
    
    if(mq_notify(sendBox, &SIGNAL) == ERROR)
    {
        perror("mq_notify");
        okay = false;
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
        if (!sendQueue.empty())
        {
            msgIn = sendQueue.front();
            //WPacket payload = TxWMsg(msgIn);
            printf("Msg received, type: %d\n", msgIn.type);
            logger.logEvent(eLevels::INFO, "received message");
            sendQueue.pop();
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
