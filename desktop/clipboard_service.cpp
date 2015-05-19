#include "clipboard_service.h"

ClipboardService::ClipboardService() : QObject()
{
    this->clipboard = QApplication::clipboard();
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
}

void ClipboardService::onClipboardChanged()
{
    if (locked) {
        locked = false;
        return;
    }
    const QMimeData * mimeData = clipboard->mimeData();
    ClipboardData data;
    QString text;
    data.dataID = (qint32)qrand();

    if (clipboardData.size() == clipboardOpacity) {
        qDebug() << "vector overflow";
        emit deleteDataFromStorage(clipboardData.takeLast().dataID);
    }

    if (mimeData->hasUrls()) {
        qDebug() << "has url: " << mimeData->text();
        data.type = "text/uri-list";
        data.data = mimeData->data(data.type);
        text = mimeData->urls().first().toString();
    }
    if (mimeData->hasImage()) {
        text = "copied image #" + QString::number(qrand());
        qDebug() << "has image: " << text;
        data.type = "image/png";
        QImage image = qvariant_cast<QImage>(mimeData->imageData());

        QImage image2;
        data.data = *imageToQByteArray(image);

        QByteArray imagestore;
       // imagestore = *imageToQByteArray(image);

//       QByteArray ba;
//       QBuffer buffer(&ba);
//       buffer.open(QIODevice::WriteOnly);
//       image.save(&buffer, "PNG");
        image2 = QImage::fromData(data.data);
        image2.save("/home/asalle/heheheheynow.png");

        qDebug() << "size: " << data.data.size();
        const char* rawdata = data.data.constData();
        qDebug() << "data:" << rawdata;
    }
    if (mimeData->hasText()) {
        qDebug() << "has text: " << mimeData->text();
        data.type = "text/plain";
        data.data = mimeData->text().toUtf8();
        text = mimeData->text();
    }
    clipboardData.prepend(data);
    emit hasDataToText(minimizeText(text), data.dataID);
    emit hasData(data.data, data.type);
}

void ClipboardService::onSettingsChoosed(int value, bool isInKB)
{
    this->clipboardOpacity = value;
    this->isInKB = isInKB;
    this->clipboardData.resize(value);
}

void ClipboardService::pushFromHosts(QByteArray data, QString type)
{
    locked = true;
    QMimeData * mimeData = new QMimeData();

    mimeData->setData(type, data);
    QImage image2 = QImage::fromData(data);
    image2.save("/home/asalle/6.png");
    clipboard->setMimeData(mimeData);

    qDebug() << "on data from outer host";
    //const QMimeData * existingData = clipboard->mimeData();
}

void ClipboardService::pushDataToClipboardFromGui(qint32 dataId)
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

QByteArray * ClipboardService::imageToQByteArray(QImage & image)
{
    QByteArray * ba = new QByteArray();
    QBuffer buffer(ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return ba;
}

QImage ClipboardService::byteArrayToImage(QByteArray & ba)
{
    return QImage::fromData(ba.constData(), "PNG");
}
