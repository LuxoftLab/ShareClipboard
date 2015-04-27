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
    if(locked)
    {
        locked = false;
        return;
    }
    const QMimeData * mimeData = clipboard->mimeData();
    ClipboardData data;
    QString text;
    data.dataID = (qint32)qrand();

    if(clipboardData.size() == clipboardOpacity) {
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
//        if(mimeData->imageData().canConvert<QByteArray>())
            //data.data = qvariant_cast<QByteArray>(mimeData->imageData());   // TODO investigate converting of image to byte array
            //data.data = mimeData->imageData().toByteArray();
            //int size = mimeData->imageData();
//        else
//            qDebug() << "cannot convert to QBytearray :(";

        QImage image = qvariant_cast<QImage>(mimeData->imageData());
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "png");
        data.data = ba;
        //emit(hasImage(image));##
    }
    if (mimeData->hasText()) {
        qDebug() << "has text: " << mimeData->text();
        data.type = "text/plain";
        data.data = mimeData->text().toUtf8();
//        text = mimeData->text();
//        emit(hasText(text));##
    }
    clipboardData.prepend(data);
    emit hasDataToText(minimizeText(text), data.dataID);
    emit hasData(data.data, data.type);
    int sz = data.data.size();//##
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
    emit setUpdatedBuffer();
    clipboard->setMimeData(mimeData);

    qDebug() << "on text from outer host";
    const QMimeData * existingData = clipboard->mimeData();
}

//##
void ClipboardService::pushText(QString text)
{
    qDebug() << "on text from outer host";
    const QMimeData * existingData = clipboard->mimeData();

    QMimeData * mimeData = new QMimeData();
    mimeData->setData("text/plain", text.toUtf8());
    emit setUpdatedBuffer();
    clipboard->setMimeData(mimeData);
}

//##
void ClipboardService::pushImage(QByteArray image)
{
    qDebug() << "on image from outer host";
    const QMimeData * existingData = clipboard->mimeData();

    //check if we already have the data
    if (existingData->hasImage() &&
            memcmp(image.constData(),
            existingData->imageData().toByteArray().constData(), image.size()) == 0){
        qDebug() << "same img"; //###
        return;
    }
    QMimeData * mimeData = new QMimeData();
    mimeData->setData("image/png", image);
    clipboard->setMimeData(mimeData);
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
