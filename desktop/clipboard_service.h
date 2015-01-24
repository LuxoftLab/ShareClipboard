#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H
#include <QClipboard>
#include <QApplication>

class ClipboardService : public QObject
{
    Q_OBJECT
public:
    ClipboardService();
    ~ClipboardService();
signals:
    void pasteText(QString text);
public slots:
    void onClipboardChanged();
private:
    QClipboard* clipboard;
    QString last_text;
};

#endif // CLIPBOARDSERVICE_H
