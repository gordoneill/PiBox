#include <QApplication>
#include "Console.h"
#include "msgQueueConst.h"
#include <signal.h>
#include "LogMgr.h"

static mqd_t sendBox, recvBox;

static void recvBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }

    Console * console = static_cast<Console *>(sv.sival_ptr);
    WMessage msgIn;
    while (errno != EAGAIN)
    {
        mq_receive(recvBox, (char *) &msgIn, 8192 , nullptr);
        console->control(msgIn);
        printf("Msg Received! Msg type: %d", msgIn.type);
    }

    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = recvBoxOnData;
    signal.sigev_notify_attributes = nullptr;
    signal.sigev_value.sival_ptr = console;

    if(mq_notify(recvBox, &signal) == ERROR)
    {
        perror("mq_notify");
        return;
    }

    return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Console * console;
    console = new Console();

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
        std::cerr << "sendBox opening failed!" << std::endl;
    }
    if (recvBox == ERROR)
    {
        mq_unlink("/recvBox");
        std::cerr << "recvBox opening failed!" << std::endl;
    }

    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = recvBoxOnData;
    signal.sigev_notify_attributes = nullptr;
    signal.sigev_value.sival_ptr = console;

    if(mq_notify(recvBox, &signal) == ERROR)
    {
        perror("mq_notify");
    }

    mq_unlink(sendQueueName);
    mq_unlink(recvQueueName);
    mq_close(sendBox);
    mq_close(recvBox);

    return a.exec();
}
