#include "clipboard_service.h"
#include <QDebug>
#include <QMimeData>
#include <QUrl>


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
    const QMimeData* mimeData = clipboard->mimeData();
    if (mimeData->hasUrls()) {
        if(mimeData->urls().first().isLocalFile()) {
         //   emit onFile(mimeData->urls().first().toLocalFile());
            qDebug() << "has file: " << mimeData->urls().first().toLocalFile();
        } else {
        //    emit onText(mimeData->text());
            qDebug() << "has url: " << mimeData->text();
        }
    } else if (mimeData->hasImage()) {
        //emit onImage(qvariant_cast<QPixmap>(mimeData->imageData()));
        qDebug() << "has image";
    } else if (mimeData->hasText()) {
        //emit onText(mimeData->text());
        qDebug() << "has text: " << mimeData->text();
    }
}
