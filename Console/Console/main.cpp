#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "AsteroidsGame.h"
#include "mqueue.h"
#include "msgQueueConst.h"
#include "WMsg_structs.h"
#include <signal.h>
#include <QEvent>
#include <QKeyEvent>
#include "LogMgr.h"

mqd_t sendBox, recvBox;

static void recvBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }

    AsteroidsGame * game = ((AsteroidsGame *) sv.sival_ptr);
    WMessage msgIn;
    while (errno != EAGAIN)
    {
        mq_receive(recvBox, (char *) &msgIn, 8192 , NULL);
        QKeyEvent key((QEvent::Type)6, Qt::Key_Enter, Qt::NoModifier);
        QCoreApplication::postEvent(game, &key);
    }

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
    QApplication a(argc, argv);

    AsteroidsGame * game = new AsteroidsGame();

//    struct mq_attr attr;
//    attr.mq_flags = 0;
//    attr.mq_maxmsg = MAX_MESSAGES;
//    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
//    attr.mq_curmsgs = 0;
//    // mailbox of messages to be sent over bluetooth
//    sendBox = mq_open(sendQueueName, O_RDWR, QUEUE_PERMISSIONS, attr);
//    // mailbox of messges received over bluetooth
//    recvBox = mq_open(recvQueueName, O_RDWR|O_NONBLOCK, QUEUE_PERMISSIONS, attr);

//    if (sendBox == ERROR)
//    {
//        mq_unlink("/sendBox");
//        std::cerr << "sendBox opening failed!" << std::endl;
//    }
//    if (recvBox == ERROR)
//    {
//        mq_unlink("/recvBox");
//        std::cerr << "recvBox opening failed!" << std::endl;
//    }

//    struct sigevent signal;
//    signal.sigev_notify = SIGEV_THREAD;
//    signal.sigev_notify_function = recvBoxOnData;
//    signal.sigev_notify_attributes = NULL;
//    signal.sigev_value.sival_ptr = game;
//    if(mq_notify(recvBox, &signal) == ERROR)
//    {
//        perror("mq_notify");
//    }

    game->showFullScreen();
    //delete game;

    mq_unlink(sendQueueName);
    mq_unlink(recvQueueName);
    mq_close(sendBox);
    mq_close(recvBox);

    return a.exec();
}
