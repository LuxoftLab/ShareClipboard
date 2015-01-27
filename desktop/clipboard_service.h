#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QPixmap>

class ClipboardService : public QObject
{
    Q_OBJECT
public:
    ClipboardService();
    ~ClipboardService();
signals:
    void hasFile(QString);
    void hasText(QString);
    void hasImage(QPixmap);
public slots:
    void onClipboardChanged();
private:
    QClipboard* clipboard;
};

#endif // CLIPBOARDSERVICE_H
