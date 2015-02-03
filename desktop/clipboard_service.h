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
    ~ClipboardService();
signals:
//    void hasFile(QString);
    void hasText(qint32, QString);
//    void hasImage(QString);
    void deleteDataFromStorage(qint32);
public slots:
    void onClipboardChanged();

    void pushDataToClipboard(ClipboardData data);
private:
    QClipboard* clipboard;
    QVector<ClipboardData>clipboardData;
    int clipboardOpacity = 2;   // TODO : write setting it from MainMenu
};

#endif // CLIPBOARDSERVICE_H
