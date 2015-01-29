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
        clipboardData.prepend(mimeData->text());
        if(mimeData->urls().first().isLocalFile()) {
            emit hasFile(mimeData->urls().first().toLocalFile());
        } else {
            emit hasText(mimeData->text());
        }
        return;
    }
    if (mimeData->hasImage()) {
//        qsrand(10000);
        QString imageName = "image #" + QString::number(qrand());
        clipboardData.prepend(imageName);
        qDebug() << "has image: " << imageName;
        emit hasImage(imageName); //qvariant_cast<QPixmap>(mimeData->imageData())); // temporary replace pixmap to text
        return;
    }
    if (mimeData->hasText()) {
        clipboardData.prepend(mimeData->text());
        qDebug() << "has text: " << mimeData->text();
        emit hasText(mimeData->text());
        return;
    }
}
