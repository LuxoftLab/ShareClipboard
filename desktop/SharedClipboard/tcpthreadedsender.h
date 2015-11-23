#ifndef TCPTHREADEDSENDER_H
#define TCPTHREADEDSENDER_H

#include <QThread>
#include<QTcpSocket>

// ###
class TcpThreadedSender : public QThread
{
public:
    TcpThreadedSender(QTcpSocket*, QByteArray & data);
};

#endif // TCPTHREADEDSENDER_H
