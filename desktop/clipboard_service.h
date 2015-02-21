#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QPixmap>
#include <QList>

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
public slots:
    void onClipboardChanged();
    void pushDataToClipboardFromGui(qint32 dataId);
    void pushDataToClipboardFromHosts(QByteArray data, QString type);
private:
    QClipboard* clipboard;
    QVector<ClipboardData>clipboardData;
    int clipboardOpacity = 2;   // TODO : write setting it from MainMenu

    QString minimizeText(QString text);
};

#endif // CLIPBOARDSERVICE_H
