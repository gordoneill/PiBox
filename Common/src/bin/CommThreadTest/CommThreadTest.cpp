#include "WMsg_encoder.h"
#include <string>
#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>

enum eSystemType {
    CONSOLE,
    CONTROLLER
};

static void recvBoxOnData(union sigval sv)
{
    logger.logEvent(eLevels::INFO, "recvBoxOnData called");
    struct mq_attr attr;
    ssize_t nr;
    WMessage payloadIn;
    mqd_t recvBox = *((mqd_t *) sv.sival_ptr);
    mq_receive(recvBox, (char *) &payloadIn, sizeof(payloadIn), NULL);
    std::cout << payloadIn.type << std::endl;
	std::cout << payloadIn.x_dir << std::endl;
	std::cout << payloadIn.y_dir << std::endl;
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
    okay = okay && logger.setLogfile("CommThreadTest.log");

    mqd_t sendBox, recvBox;
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open("sendBox", O_RDWR, 0, 0);
    // mailbox of messges received over bluetooth
    recvBox = mq_open("recvBox", O_RDWR, 0, 0);
    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = recvBoxOnData;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &recvBox;
    mq_notify(recvBox, &sev);
    
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

    	okay = okay && mq_send(sendBox, (char *) &msgIn, sizeof(msgIn), 1) == OK;
    }
}