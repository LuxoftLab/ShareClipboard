#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QThread>

//#include "tcp_package.h"
#include "sharedPackages/package.h"

class Connection : public QThread
{
    /** @brief Represents abstract container for QTcpSocket
     *          Abstract class generalizes common behaviour
     *
     *  Class provides an interface for ServerConnection and ClientConnection,
     *  generalizes common functions, as downloadMore() and dispatch()
     *  and some common members, as socket, hand, transferFinished
     */

    Q_OBJECT
protected:
    void downloadMore(QByteArray& whole, QTcpSocket * inSocket);
    /**
     * @brief dispatch function for incoming packages
     * A pure virtual member
     * @param in of class QDataStream, passed by reference
     */
    virtual void dispatch(QDataStream & in) = 0;

    QTcpSocket *socket;
    TcpPackage * hand;
    bool transferFinished;
    qint32 currenFiletSize;
    qint32 packt;
    QByteArray file;
public:
    Connection(QTcpSocket * socket);
    Connection();
    QHostAddress getIpv4();
    QHostAddress localAddress();

    const QTcpSocket *getSocket() const;
public slots:
    void onData();
};

#endif // CONNECTION_H
