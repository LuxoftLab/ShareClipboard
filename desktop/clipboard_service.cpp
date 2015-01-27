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
        qDebug() << "has url: " << mimeData->text();
        if(mimeData->urls().first().isLocalFile()) {
            emit hasFile(mimeData->urls().first().toLocalFile());
        } else {
            emit hasText(mimeData->text());
        }
        return;
    }
    if (mimeData->hasImage()) {
        emit hasImage(qvariant_cast<QPixmap>(mimeData->imageData()));
        qDebug() << "has image";
        return;
    }
    if (mimeData->hasText()) {
        emit hasText(mimeData->text());
        qDebug() << "has text: " << mimeData->text();
        return;
    }
}
