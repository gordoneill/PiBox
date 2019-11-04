#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "asteroids.h"
//#include "mqueue.h"
//#include "msgQueueConst.h"

//mqd_t sendBox, recvBox;

//static void recvBoxOnData(union sigval sv)
//{
//    WMessage msgIn;
//    while (errno != EAGAIN)
//    {
//        mq_receive(recvBox, (char *) &msgIn, 8192 , NULL);
//        recvQueue.push(msgIn);
//    }
//    struct sigevent signal;
//    signal.sigev_notify = SIGEV_THREAD;
//    signal.sigev_notify_function = recvBoxOnData;
//    signal.sigev_notify_attributes = NULL;
//    if(mq_notify(sendBox, &signal) == ERROR)
//    {
//        perror("mq_notify");
//    }
//    return;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Asteroids * game = new Asteroids();
    game->showFullScreen();

//    struct mq_attr attr;
//    attr.mq_flags = 0;
//    attr.mq_maxmsg = MAX_MESSAGES;
//    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
//    attr.mq_curmsgs = 0;
//    // mailbox of messages to be sent over bluetooth
//    sendBox = mq_open(sendQueueName, O_RDWR, QUEUE_PERMISSIONS, attr);
//    // mailbox of messges received over bluetooth
//    recvBox = mq_open(recvQueueName, O_RDWR, QUEUE_PERMISSIONS, attr);

//    if (sendBox == ERROR)
//    {
//        mq_unlink("/sendBox");
//        okay = false;
//        logger.logEvent(eLevels::FATAL, "sendBox opening failed!");
//        std::cerr << "sendBox opening failed!" << std::endl;
//    }
//    if (recvBox == ERROR)
//    {
//        mq_unlink("/recvBox");
//        okay = false;
//        logger.logEvent(eLevels::FATAL, "recvBox opening failed!");
//        std::cerr << "recvBox opening failed!" << std::endl;
//    }

//    struct sigevent signal;
//    signal.sigev_notify = SIGEV_THREAD;
//    signal.sigev_notify_function = recvBoxOnData;
//    signal.sigev_notify_attributes = NULL;
//    if(mq_notify(recvBox, &signal) == ERROR)
//    {
//        perror("mq_notify");
//        okay = false;
//    }

//    mq_unlink(sendQueueName);
//    mq_unlink(recvQueueName);
//    mq_close(sendBox);
//    mq_close(recvBox);

    return a.exec();
}
