#include "WMsg_encoder.h"
#include "LogMgr.h"
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
    okay = okay && logger.setLogfile("Logs/CommThreadTest.log");

    sleep(5); // time for mailboxes to be setup
    mqd_t sendBox, recvBox;
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open("/sendBox", O_WRONLY);
    // mailbox of messges received over bluetooth
    recvBox = mq_open("/recvBox", O_RDONLY);

    if (sendBox == ERROR || recvBox == ERROR)
    {
        okay = false;
        logger.logEvent(eLevels::FATAL, "sendBox or recvBox opening failed!");
        std::cerr << "sendBox or recvBox opening failed!" << std::endl;
    }

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
        sleep(5);
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
    }

    mq_unlink("/sendBox");
    mq_unlink("/recvBox");
    mq_close(sendBox);
    mq_close(recvBox);
}
