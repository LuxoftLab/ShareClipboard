#include "clipboard_service.h"
#include <QDebug>
#include <QMimeData>

ClipboardService::ClipboardService()
{
    this->clipboard = QApplication::clipboard();
    QObject::connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
}

ClipboardService::~ClipboardService()
{
}

void ClipboardService::onClipboardChanged()
{
    QString text;
    const QMimeData* data = clipboard->mimeData();
    if (data->hasText())
    {
        text = data->text();
        clipboard->clear();
        qDebug()<<"has text:";
        qDebug()<< text;
    }
}
