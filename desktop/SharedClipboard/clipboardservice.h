#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H

#include <QObject>
#include <QByteArray>


#include "tcppackages.h"

class ClipboardService : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardService(QObject *parent = 0);

signals:
    void clipboardChanged(QByteArray & data);

public slots:
    void updateClipboard(TcpPackage type, QByteArray & data);
};

#endif // CLIPBOARDSERVICE_H
