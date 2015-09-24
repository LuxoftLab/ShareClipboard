#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H

#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QPixmap>
#include <QList>
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include <QMimeData>
#include <QUrl>
#include <QDateTime>
#include <QFileInfo>

struct ClipboardData {
    qint32 dataID;
    QByteArray data;
    QString type;
};

class ClipboardService : public QObject
{
    Q_OBJECT
public:
    ClipboardService();
signals:
    void hasDataToText(QString, qint32);
    void hasData(QByteArray, QString);
    void deleteDataFromStorage(qint32);
    void clipboardRefreshed(QString type, QByteArray & data);
public slots:
    void onClipboardChanged();
    void onSettingsChoosed(int value, bool isInKB);
    void pushDataToClipboardFromGui(qint32 dataId);
    void pushFromHosts(QByteArray data, QString type);
    void turnSharing();
private:
    QClipboard* clipboard;
    QVector<ClipboardData>clipboardData;
    int clipboardOpacity = 10;
    bool isInKB;
    bool locked;
    bool sharingOn;

    QString minimizeText(QString text);
    QByteArray * imageToQByteArray(QImage&);
    QImage byteArrayToImage(QByteArray&);
};

#endif // CLIPBOARDSERVICE_H
