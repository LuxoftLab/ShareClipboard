#ifndef CONNECTION_H
#define CONNECTION_H

#include <QUdpSocket>

class Connection
{
    QUdpSocket* socket;
public:
    Connection(QUdpSocket * socket);
    qint32 getIpc4();
};

#endif // CONNECTION_H
