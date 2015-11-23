#include "clipboardservice.h"

ClipboardService::ClipboardService(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged,this, &ClipboardService::getClipboardData);
}

ClipboardService::~ClipboardService()
{
    delete clipboard;
}

void ClipboardService::updateClipboard(TcpPackage type, QByteArray &data)
{

}

void ClipboardService::getClipboardData()
{

}
