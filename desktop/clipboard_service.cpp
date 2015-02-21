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
    const QMimeData* mimeData = clipboard->mimeData();
    ClipboardData data;

    if(clipboardData.size() == clipboardOpacity) {
        emit deleteDataFromStorage(clipboardData.at(clipboardData.size() - 1).dataID);
        clipboardData.remove(clipboardData.size()- 1);
    }
    if (mimeData->hasUrls()) {
        qDebug() << "has url: " << mimeData->text();
        data.dataID = (qint32)qrand();
        data.type = "text/uri-list";
        data.data = mimeData->data(data.type);
        clipboardData.prepend(data);
        if(mimeData->urls().first().isLocalFile()) {
            emit hasFile(data.dataID, mimeData->urls().first().toLocalFile());
        } else {
            emit hasText(data.dataID, mimeData->text());
        }
        return;
    }
    if (mimeData->hasImage()) {
        QString imageName = "copied image #" + QString::number(qrand());
        data.dataID = (qint32)qrand();
        data.type = "image/ *";
        data.data = mimeData->data(data.type);
        clipboardData.prepend(data);
        qDebug() << "has image: " << imageName;
        emit hasImage(data.dataID, imageName); //qvariant_cast<QPixmap>(mimeData->imageData())); // temporary replace pixmap to text
        return;
    }
    if (mimeData->hasText()) {
        qDebug() << "has text: " << mimeData->text();
        data.dataID = (qint32)qrand();
        data.type = "text/plain";
        data.data = mimeData->data(data.type);
        clipboardData.prepend(data);

        // TODO add split method

        emit hasText(data.dataID, mimeData->text());
    }

    qDebug() << clipboardData.size();
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
