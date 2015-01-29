#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QPixmap>
#include <QList>

class ClipboardService : public QObject
{
    Q_OBJECT
public:
    ClipboardService();
    ~ClipboardService();
signals:
    void hasFile(QString);
    void hasText(QString);
    void hasImage(QString);
public slots:
    void onClipboardChanged();
private:
    QClipboard* clipboard;
    QList<QString> clipboardData;
};

#endif // CLIPBOARDSERVICE_H
