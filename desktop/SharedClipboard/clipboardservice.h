#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H

#include <QObject>
#include <QClipboard>
#include <QApplication>
#include <QByteArray>


#include "tcppackages.h"

class ClipboardService : public QObject
{
    Q_OBJECT

    QClipboard * clipboard = NULL;
    bool sharing = true; // switches sharing on and off
public:
    explicit ClipboardService(QObject *parent = 0);
    ~ClipboardService();

signals:
    void clipboardChanged(QByteArray & data);

public slots:
    void updateClipboard(TcpPackage type, QByteArray & data);
    void getClipboardData();
};

#endif // CLIPBOARDSERVICE_H
