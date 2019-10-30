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
    
    uint32_t x_dir = 0;
    uint32_t y_dir = 50;
    while (okay)
    {
    	WMessage msgOut;
    	if (systemType == eSystemType::CONTROLLER)
    	{
    		msgOut.type = eMsgTypes::DIRECTION;
    		msgOut.x_dir = x_dir++;
    		msgOut.y_dir = y_dir++;
    	}
    	else
    	{
    		msgOut.type = eMsgTypes::STATUS;
    	}

    	okay = okay && mq_send(sendBox, (char *) &msgOut, sizeof(msgOut), 1) == OK;
        logger.logEvent(eLevels::INFO, "Placed something in sendBox. okay: %d", okay);
        sleep(1);
    }

    mq_unlink("/sendBox");
    mq_unlink("/recvBox");
    mq_close(sendBox);
    mq_close(recvBox);
}