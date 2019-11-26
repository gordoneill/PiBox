#include "WMsg_encoder.h"
#include "msgQueueConst.h"
#include "LogMgr.h"
#include <string.h>
#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

mqd_t sendBox, recvBox;

static void recvBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Entering MQStat.mq_curmsgs: %ld\n", MQStat.mq_curmsgs);
    
    WMessage msgIn;
    while (errno != EAGAIN)
    {
        mq_receive(recvBox, (char *) &msgIn, 8192 , NULL);
        // DO SOMETHING WITH THE MSG
    }
    
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    
    printf("On Exiting MQStat.mq_curmsgs: %ld\n", MQStat.mq_curmsgs);

    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = recvBoxOnData;
    signal.sigev_notify_attributes = NULL;
    if(mq_notify(recvBox, &signal) == ERROR)
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

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open(sendQueueName, O_RDWR, QUEUE_PERMISSIONS, attr);
    // mailbox of messges received over bluetooth
    recvBox = mq_open(recvQueueName, O_RDWR|O_NONBLOCK, QUEUE_PERMISSIONS, attr);

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

    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = recvBoxOnData;
    signal.sigev_notify_attributes = NULL;
    if(mq_notify(recvBox, &signal) == ERROR)
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

    	okay = okay && mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0) == OK;
        logger.logEvent(eLevels::INFO, "Placed something in sendBox. okay: %d", okay);
        sleep(0.5);
    }

    mq_unlink(sendQueueName);
    mq_unlink(recvQueueName);
    mq_close(sendBox);
    mq_close(recvBox);
}
