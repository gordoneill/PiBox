#include "WMsg_encoder.h"
#include "LogMgr.h"
#include <string.h>
#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MQ_MSG_SIZE 256

extern int errno;
int errnum;

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

static void recvBoxOnData(union sigval sv)
{
    WMessage payloadIn;
    mqd_t recvBox = *((mqd_t *) sv.sival_ptr);

    while (errno != EAGAIN)
    {
        mq_receive(recvBox, (char *) &payloadIn, sizeof(payloadIn), NULL);
        if (errno != EAGAIN)
        {
            //sendQueue.push(payloadIn);
            std::cout << "Test: got a rx msg" << std::endl;
        }
    }

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = recvBoxOnData;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &recvBox;
    
    if (mq_notify(recvBox, &sev) != OK)
    {
        std::cerr << "mq_notify failed!" << std::endl;
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
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
    okay = okay && logger.setLogfile("Logs/CommThreadTest.log");

    mqd_t sendBox, recvBox;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open("/sendBox", O_RDWR, QUEUE_PERMISSIONS, attr);
    // mailbox of messges received over bluetooth
    recvBox = mq_open("/recvBox", O_RDWR, QUEUE_PERMISSIONS, attr);

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
    sev.sigev_notify_function = recvBoxOnData;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &recvBox;
    
    if (mq_notify(recvBox, &sev) != OK)
    {
        okay = false;
        logger.logEvent(eLevels::FATAL, "mq_notify failed!");
        std::cerr << "mq_notify failed!" << std::endl;
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
    }
    
    uint32_t x_dir = 0;
    uint32_t y_dir = 50;
    while (okay)
    {
    	// WMessage msgOut;
    	// if (systemType == eSystemType::CONTROLLER)
    	// {
    	// 	msgOut.type = eMsgTypes::DIRECTION;
    	// 	msgOut.x_dir = x_dir++;
    	// 	msgOut.y_dir = y_dir++;
    	// }
    	// else
    	// {
    	// 	msgOut.type = eMsgTypes::STATUS;
    	// }

        std::string msgOut = "Hello";
        //const char * msgOutC = msgOut.c_str();

    	okay = okay && mq_send(sendBox, msgOut.c_str(), msgOut.length()+1, 1) == OK;
        logger.logEvent(eLevels::INFO, "Placed something in sendBox. okay: %d", okay);
        sleep(1);
    }

    mq_unlink("/sendBox");
    mq_unlink("/recvBox");
    mq_close(sendBox);
    mq_close(recvBox);
}