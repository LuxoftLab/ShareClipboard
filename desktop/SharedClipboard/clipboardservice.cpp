#include "clipboardservice.h"

ClipboardService::ClipboardService(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged,this, &ClipboardService::getClipboardData);
}

ClipboardService::~ClipboardService()
{

}

void ClipboardService::updateClipboard(TcpPackage type, QByteArray &data)
{

}

void ClipboardService::getClipboardData()
{
    QMimeData const * data = clipboard->mimeData();
    QString text = data->text();
    QByteArray * temp = new QByteArray(text.toUtf8().constData(), text.size());
    emit clipboardChanged(*temp);
}
