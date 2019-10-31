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

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

extern int errno;
int errnum;
mqd_t sendBox, recvBox; 
char Message[MAX_MQ_MSG_SIZE+1];
struct sigevent SIGNAL;

static void recvBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Entering MQStat.mq_curmsgs: %ld\n",MQStat.mq_curmsgs);
    
    ssize_t noOfBytesRx = mq_receive(recvBox, Message, MAX_MQ_MSG_SIZE+1 , 0);

    printf("Received: %s\n", Message);
    
    if(noOfBytesRx == ERROR)
    {
        perror("mq_receive");
        return;
    }
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Exiting MQStat.mq_curmsgs: %ld\n",MQStat.mq_curmsgs);
    if(mq_notify(recvBox, &SIGNAL) == ERROR)
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

    SIGNAL.sigev_notify = SIGEV_THREAD;
    SIGNAL.sigev_notify_function = recvBoxOnData;
    SIGNAL.sigev_notify_attributes = NULL;
    
    if(mq_notify(recvBox, &SIGNAL) == ERROR)
    {
        perror("mq_notify");
        okay = false;
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

        std::string out = ToString(msgOut);
    	okay = okay && mq_send(sendBox, out.c_str(), out.length()+1, 1) == OK;
        logger.logEvent(eLevels::INFO, "Placed something in sendBox. okay: %d", okay);
        sleep(1);
    }

    mq_unlink("/sendBox");
    mq_unlink("/recvBox");
    mq_close(sendBox);
    mq_close(recvBox);
}