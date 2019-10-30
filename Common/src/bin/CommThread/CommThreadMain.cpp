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

std::queue<WMessage> sendQueue;

static void sendBoxOnData(union sigval sv)
{
    std::cout << "something is sending!" << std::endl;
    WMessage payloadIn;
    mqd_t sendBox = *((mqd_t *) sv.sival_ptr);

    while (errno != EAGAIN)
    {
        mq_receive(sendBox, (char *) &payloadIn, 8192, NULL);
        if (errno != EAGAIN)
        {
            sendQueue.push(payloadIn);
        }
    }

    // Re-register for new messages on Queue
    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = sendBoxOnData;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = sv.sival_ptr;
    if (mq_notify(sendBox, &sev) < 0)
    {
        std::cerr << "Error during Reregister in msq_notify : " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
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

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = sendBoxOnData;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &sendBox;
    
    if (mq_notify(sendBox, &sev) != OK)
    {
        okay = false;
        logger.logEvent(eLevels::FATAL, "mq_notify failed!");
        std::cerr << "mq_notify failed!" << std::endl;
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
    }

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

    mq_unlink("/sendBox");
    mq_unlink("/recvBox");
    mq_close(sendBox);
    mq_close(recvBox);

    return okay;
}
