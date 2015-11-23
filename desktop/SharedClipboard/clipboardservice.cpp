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
    QByteArray * temp = new QByteArray("azaza temp bytearray", 21);
    emit clipboardChanged(*temp);
}
