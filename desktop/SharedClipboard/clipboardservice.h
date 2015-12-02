#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H

#include <QObject>
#include <QClipboard>
#include <QApplication>
#include <QByteArray>
#include <QMimeData>
#include <QBuffer>
#include <QImage>
#include <QUrl>


#include "tcppackages.h"

class ClipboardService : public QObject
{
    Q_OBJECT

    QClipboard * clipboard = NULL;
    bool sharing = true; // switches sharing on and off
    bool sent = false; // prevents infinite recursive duplication
public:
    explicit ClipboardService(QObject *parent = 0);
    ~ClipboardService();

signals:
    void clipboardChanged(QByteArray & data);
    void hasUrls(QList<QUrl>);

public slots:
    void updateClipboard(TcpPackage type, QByteArray & data);
    void getClipboardData();
private:
    QByteArray * toByteArray(QImage&);
    QImage * fromByteArray(QByteArray&);
};

#endif // CLIPBOARDSERVICE_H
