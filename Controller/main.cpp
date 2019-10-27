/* Gordon O'Neill
 * ECE 3574 Applied Software Design HW 5
 * gordo95
 * 905793765
 */
#include <QString>
#include <QTextStream>
#include <pthread.h>
#include <errno.h>
#include <mqueue.h>
#include <cmath>
mqd_t mailbox0, mailbox1, mailbox2, mailbox3, mailbox4, mailbox5, mailbox6;
QTextStream qtCout(stdout);
QTextStream qtCerr(stderr);
using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        if(argc != 3)
        {
            //need 3 args
            throw 5;
        }
        float node_temp, up_temp, down_temp, child_a_temp, child_b_temp;
        int mailbox;
        char* incoming = new char[8192];
        char* outgoing = new char();
        bool done = false;
        char direction = 'd';
        QString tempQString;
        QString start = "start"; // start signal
        QString finished = "finished"; // done signal
        bool floatcheck1, floatcheck2;
        //check to see that they are numbers
        mailbox = QString(argv[1]).toInt(&floatcheck1);
        node_temp = QString(argv[2]).toFloat(&floatcheck2);
        if(!(floatcheck1&floatcheck2))
        {
            throw 1;
        }
        if(mailbox < 0 || mailbox > 6)
        {
            //need correct mailbox ID
            throw 0;
        }
        //cases for each node ID
        switch(mailbox)
        {
            case 0:
                mailbox0 = mq_open("/70", O_RDWR|O_CREAT|O_EXCL, 0666, 0); //open nessesary mailboxes (itself and children)
                mailbox1 = mq_open("/71", O_RDWR, 0, 0);
                mailbox2 = mq_open("/72", O_RDWR, 0, 0);
                //if any fail, exit
                if(mailbox0 == -1)
                {
                    throw 2;
                    mq_unlink("/70");
                }
                if(mailbox1 == -1)
                {
                    throw 2;
                    mq_unlink("/71");
                }
                if(mailbox2 == -1)
                {
                    throw 2;
                    mq_unlink("/72");
                }
                mq_send(mailbox1, start.toLocal8Bit().data(), strlen(start.toLocal8Bit().data())+1, 1);
                mq_send(mailbox2, start.toLocal8Bit().data(), strlen(start.toLocal8Bit().data())+1, 1);
                //convert from QString to char* array, send the size of it + 1
                outgoing = QString::number(node_temp).toLocal8Bit().data();
                mq_send(mailbox1, outgoing, strlen(outgoing)+1, 1);
                mq_send(mailbox2, outgoing, strlen(outgoing)+1, 1);
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done)
                {
                    mq_receive(mailbox0, incoming, 8192, 0);
                    child_a_temp = QString(incoming).toFloat();
                    mq_receive(mailbox0, incoming, 8192, 0);
                    child_b_temp = QString(incoming).toFloat();

                    up_temp = (node_temp + child_a_temp + child_b_temp)/3.0;
                    down_temp = up_temp;
                    if (abs(up_temp - node_temp) < 0.01)
                    {
                        node_temp = down_temp;
                        done = true;
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                        mq_send(mailbox1, finished.toLocal8Bit().data(), strlen(finished.toLocal8Bit().data())+1, 1);
                        mq_send(mailbox2, finished.toLocal8Bit().data(), strlen(finished.toLocal8Bit().data())+1, 1);
                    }
                    else
                    {
                        node_temp = down_temp;
                        outgoing = QString::number(down_temp).toLocal8Bit().data();
                        mq_send(mailbox1, outgoing, strlen(outgoing)+1, 1);
                        mq_send(mailbox2, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            case 1:
                mailbox1 = mq_open("/71", O_RDWR|O_CREAT|O_EXCL, 0666, 0);
                mailbox3 = mq_open("/73", O_RDWR, 0, 0);
                mailbox4 = mq_open("/74", O_RDWR, 0, 0);
                if(mailbox1 == -1)
                {
                    throw 2;
                    mq_unlink("/71");
                }
                if(mailbox3 == -1)
                {
                    throw 2;
                    mq_unlink("/73");
                }
                if(mailbox4 == -1)
                {
                    throw 2;
                    mq_unlink("/74");
                }
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done)
                {
                    mq_receive(mailbox1, incoming, 8192, 0);
                    tempQString = QString::fromLocal8Bit(incoming);
                    if (tempQString == "finished")
                    {
                        done = true;
                        mq_send(mailbox3, finished.toLocal8Bit().data(), strlen(finished.toLocal8Bit().data())+1, 1);
                        mq_send(mailbox4,  finished.toLocal8Bit().data(), strlen(finished.toLocal8Bit().data())+1, 1);
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                    }
                    else if (tempQString == "start")
                    {
                        //open the parent
                        mailbox0 = mq_open("/70", O_WRONLY, 0, 0);
                        mq_send(mailbox3, start.toLocal8Bit().data(), strlen(start.toLocal8Bit().data())+1, 1);
                        mq_send(mailbox4, start.toLocal8Bit().data(), strlen(start.toLocal8Bit().data())+1, 1);
                    }

                    else if (direction == 'd')
                    {
                        down_temp = (node_temp + tempQString.toFloat())/2.0;
                        node_temp = down_temp;
                        direction = 'u';

                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox3, outgoing, strlen(outgoing)+1, 1);
                        mq_send(mailbox4, outgoing, strlen(outgoing)+1, 1);
                    }
                    else
                    {
                        child_a_temp = tempQString.toFloat();
                        mq_receive(mailbox1, incoming, 8192, 0);
                        child_b_temp = QString(incoming).toFloat();

                        up_temp = (node_temp + child_a_temp + child_b_temp)/3.0;
                        node_temp = up_temp;
                        direction = 'd';

                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox0, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            case 2:
                mailbox2 = mq_open("/72", O_RDWR|O_CREAT|O_EXCL, 0666, 0);
                mailbox5 = mq_open("/75", O_RDWR, 0, 0);
                mailbox6 = mq_open("/76", O_RDWR, 0, 0);
                if(mailbox2 == -1)
                {
                    throw 2;
                    mq_unlink("/72");
                }
                if(mailbox5 == -1)
                {
                    throw 2;
                    mq_unlink("/75");
                }
                if(mailbox6 == -1)
                {
                    throw 2;
                    mq_unlink("/76");
                }
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done)
                {
                    mq_receive(mailbox2, incoming, 8192, 0);
                    tempQString = QString::fromLocal8Bit(incoming);
                    if (tempQString == "finished")
                    {
                        done = true;
                        mq_send(mailbox5, finished.toLocal8Bit().data(), strlen(finished.toLocal8Bit().data())+1, 1);
                        mq_send(mailbox6, finished.toLocal8Bit().data(), strlen(finished.toLocal8Bit().data())+1, 1);
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                    }
                    else if (tempQString == "start")
                    {
                        //open the parent
                        mailbox0 = mq_open("/70", O_WRONLY, 0, 0);
                        mq_send(mailbox5, start.toLocal8Bit().data(), strlen(start.toLocal8Bit().data())+1, 1);
                        mq_send(mailbox6, start.toLocal8Bit().data(), strlen(start.toLocal8Bit().data())+1, 1);
                    }
                    //going down
                    else if (direction == 'd')
                    {
                        down_temp = (node_temp + tempQString.toFloat())/2.0;
                        node_temp = down_temp;
                        direction = 'u';
                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox5, outgoing, strlen(outgoing)+1, 1);
                        mq_send(mailbox6, outgoing, strlen(outgoing)+1, 1);
                    }
                    //going up
                    else
                    {
                        child_a_temp = tempQString.toFloat();
                        mq_receive(mailbox2, incoming, 8192, 0);
                        child_b_temp = QString(incoming).toFloat();

                        up_temp = (node_temp + child_a_temp + child_b_temp)/3.0;
                        node_temp = up_temp;
                        direction = 'd';

                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox0, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            case 3:
                mailbox3 = mq_open("/73", O_RDWR|O_CREAT|O_EXCL, 0666, 0);
                if(mailbox3 == -1)
                {
                    throw 2;
                    mq_unlink("/73");
                }
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done)
                {
                    mq_receive(mailbox3, incoming, 8192, 0);
                    tempQString = QString::fromLocal8Bit(incoming);
                    if (tempQString == "finished")
                    {
                        done = true;
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                    }
                    else if (tempQString == "start")
                    {
                        //open the parent
                        mailbox1 = mq_open("/71", O_RDWR, 0, 0);
                    }

                    else
                    {
                        node_temp = (node_temp + tempQString.toFloat())/2.0;
                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox1, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            case 4:
                mailbox4 = mq_open("/74", O_RDWR|O_CREAT|O_EXCL, 0666, 0);
                if(mailbox4 == -1)
                {
                    throw 2;
                    mq_unlink("/74");
                }
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done) {
                    mq_receive(mailbox4, incoming, 8192, 0);
                    tempQString = QString::fromLocal8Bit(incoming);
                    if (tempQString == "finished")
                    {
                        done = true;
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                    }
                    else if (tempQString == "start")
                    {
                        //open the parent
                        mailbox1 = mq_open("/71", O_RDWR, 0, 0);
                    }
                    else
                    {
                        node_temp = (node_temp + tempQString.toFloat())/2.0;
                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox1, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            case 5:
                mailbox5 = mq_open("/75", O_RDWR|O_CREAT|O_EXCL, 0666, 0);
                if(mailbox5 == -1)
                {
                    throw 2;
                    mq_unlink("/75");
                }
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done)
                {
                    mq_receive(mailbox5, incoming, 8192, 0);
                    tempQString = QString::fromLocal8Bit(incoming);
                    if (tempQString == "finished")
                    {
                        done = true;
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                    }
                    else if (tempQString == "start")
                    {
                        //open the parent
                        mailbox2 = mq_open("/72", O_RDWR, 0, 0);
                    }

                    else {
                        node_temp = (node_temp + tempQString.toFloat())/2.0;
                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox2, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            case 6:
                mailbox6 = mq_open("/76", O_RDWR|O_CREAT|O_EXCL, 0666, 0);
                if(mailbox6 == -1)
                {
                    throw 2;
                    mq_unlink("/76");
                }
                qtCout << "Process #" << mailbox << ": initial temperature " << node_temp << endl;
                while(!done)
                {
                    mq_receive(mailbox6, incoming, 8192, 0);
                    tempQString = QString::fromLocal8Bit(incoming);
                    if (tempQString == "finished")
                    {
                        done = true;
                        qtCout << "Process #" << mailbox << ": final temperature " << node_temp << endl;
                    }
                    else if (tempQString == "start")
                    {
                        //open the parent
                        mailbox2 = mq_open("/72", O_RDWR, 0, 0);
                    }
                    else
                    {
                        node_temp = (node_temp + tempQString.toFloat())/2.0;
                        outgoing = QString::number(node_temp).toLocal8Bit().data();
                        mq_send(mailbox2, outgoing, strlen(outgoing)+1, 1);
                        qtCout << "Process #" << mailbox << ": current temperature " << node_temp << endl;
                    }
                }
                break;
            default:
                break;
        }
    }
    //error handleing
    catch (int errorcode)
    {
        switch(errorcode)
        {
            case 0: qtCerr << "Incorrect mailbox ID, only 0-6" << endl;
                    break;
            case 1: qtCerr << "Incorrect arguments, only numbers!" << endl;
            break;
            case 2: qtCerr << "Failed to open a mailbox" << endl;
                    break;
            case 3:
            case 4:
            case 5: qtCerr << "Invalid Arguments, only 2!" << endl;
                    break;
            default: break;
        }
        //unlink and close all
        mq_unlink("/70");
        mq_unlink("/71");
        mq_unlink("/72");
        mq_unlink("/73");
        mq_unlink("/74");
        mq_unlink("/75");
        mq_unlink("/76");
        mq_close(mailbox0);
        mq_close(mailbox1);
        mq_close(mailbox2);
        mq_close(mailbox3);
        mq_close(mailbox4);
        mq_close(mailbox5);
        mq_close(mailbox6);
        exit(-1);
    }
    //unlink and close all
    mq_unlink("/70");
    mq_unlink("/71");
    mq_unlink("/72");
    mq_unlink("/73");
    mq_unlink("/74");
    mq_unlink("/75");
    mq_unlink("/76");
    mq_close(mailbox0);
    mq_close(mailbox1);
    mq_close(mailbox2);
    mq_close(mailbox3);
    mq_close(mailbox4);
    mq_close(mailbox5);
    mq_close(mailbox6);
    return 0;
}
