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
//    if (mimeData->hasUrls()) {
//        qDebug() << "has url: " << mimeData->text();
//        qint32 dataId = (qint32)qrand();
//        clipboardData.insert(dataId, mimeData->text());
//        if(mimeData->urls().first().isLocalFile()) {
//            emit hasFile(mimeData->urls().first().toLocalFile());
//        } else {
//            emit hasText(mimeData->text(),);
//        }
//        return;
//    }
//    if (mimeData->hasImage()) {
//        qsrand(10000);
//        QString imageName = "copied image #" + QString::number(qrand());
//        clipboardData.prepend(imageName);
//        qDebug() << "has image: " << imageName;
//        emit hasImage(imageName); //qvariant_cast<QPixmap>(mimeData->imageData())); // temporary replace pixmap to text
//        return;
//    }
    if (mimeData->hasText()) {
//        qDebug() << "has text: " << mimeData->text();
//        qint32 dataId = (qint32)qrand();
//        clipboardData.insert(dataId, mimeData->text());
//        emit hasText(dataId, mimeData->text());
//        //return;
        qDebug() << "has text: " << mimeData->text();
        data.dataID = (qint32)qrand();
        data.type = "text/plain";
        data.data = mimeData->data(data.type);
        clipboardData.prepend(data);

        // TODO add split method

        emit hasText(data.dataID, mimeData->text());
    }
}

void ClipboardService::pushDataToClipboard(ClipboardData data)
{
    QMimeData * mimeData = new QMimeData();
    mimeData->setData(data.type, data.data);
    clipboard->setMimeData(mimeData);
}
