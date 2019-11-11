#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "AsteroidsGame.h"
//#include "mqueue.h"
//#include "msgQueueConst.h"

//mqd_t sendBox, recvBox;

//static void recvBoxOnData(union sigval sv)
//{
//    AsteroidsGame * game = (AsteroidsGame *) sv.sival_ptr;
//    WMessage msgIn;
//    while (errno != EAGAIN)
//    {
//        mq_receive(recvBox, (char *) &msgIn, 8192 , NULL);
//        if(msgIn.type == eMsgTypes::BUTTON && msgIn.button == eButtons::BTN_A && btnState == true)
//        {
//            QKeyEvent * ev = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space);
//            QCoreApplication::postEvent(game, ev);
//        }
//    }
//    struct sigevent signal;
//    signal.sigev_notify = SIGEV_THREAD;
//    signal.sigev_notify_function = recvBoxOnData;
//    signal.sigev_notify_attributes = NULL;
//    signal.sigev_ptr = sv.sival_ptr;
//    if(mq_notify(sendBox, &signal) == ERROR)
//    {
//        perror("mq_notify");
//    }
//    return;
//}

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
//    signal.sigev_ptr = game;
//    if(mq_notify(recvBox, &signal) == ERROR)
//    {
//        perror("mq_notify");
//        okay = false;
//    }

    game->showFullScreen();
    //delete game;

//    mq_unlink(sendQueueName);
//    mq_unlink(recvQueueName);
//    mq_close(sendBox);
//    mq_close(recvBox);

    return a.exec();
}
