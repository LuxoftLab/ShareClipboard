#include "clipboard_service.h"
#include <QDebug>
#include <QMimeData>
#include <QUrl>


ClipboardService::ClipboardService() : QObject()
{
    this->clipboard = QApplication::clipboard();
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
}

void ClipboardService::onClipboardChanged()
{
    const QMimeData * mimeData = clipboard->mimeData();
    ClipboardData data;
    QString text;
    data.dataID = (qint32)qrand();

    if(clipboardData.size() == clipboardOpacity) {
        qDebug() << clipboardData.last().dataID;
        emit deleteDataFromStorage(clipboardData.takeLast().dataID);
    }

    if (mimeData->hasUrls()) {
        qDebug() << "has url: " << mimeData->text();
        data.type = "text/uri-list";
        text = mimeData->urls().first().toString();
    }
    if (mimeData->hasImage()) {
        text = "copied image #" + QString::number(qrand());
        qDebug() << "has image: " << imageName;
        data.type = "image/ *";
    }
    if (mimeData->hasText()) {
        qDebug() << "has text: " << mimeData->text();
        data.type = "text/plain";
        text = mimeData->text();
    }
    data.data = mimeData->data(data.type);
    clipboardData.prepend(data);
    emit hasDataToText(minimizeText(text), data.dataID);
    emit hasData(data.data, data.type);

}

void ClipboardService::pushDataToClipboardFromHosts(QByteArray data, QString type)
{
    qDebug() << "on text from outer host";
    QMimeData * data = new QMimeData();
    data->setData(type, data);
    clipboard->setMimeData(data);
}

void ClipboardService::pushDataToClipboard(qint32 dataId)
{
    qDebug() << "on item dbclick";

    for(int i = 0; i < clipboardData.size(); i++) {
        if(clipboardData.at(i).dataID == dataId) {
            QMimeData * data = new QMimeData();
            data->setData(clipboardData.at(i).type, clipboardData.at(i).data);
            clipboard->setMimeData(data);
            return;
        }
    }
}

QString ClipboardService::minimizeText(QString text)
{
    const int MAX_STR_LEN = 50;

    int index = text.indexOf('\n');
    int size = text.size();

    if( index >= 0 && index < MAX_STR_LEN)
    {
        if(index < 3 ) { index = text.indexOf('\n',3); }
        text = text.mid(0, index);
        text.append("...");
    }
    else if(text.size() > MAX_STR_LEN)
    {
        text = text.mid(0, MAX_STR_LEN);
        text.append("...");
    }
    return text;
}
