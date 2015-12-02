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
    if(type == TcpPackage::TXT){
        clipboard->setText(QString(data));
    } else if(type == TcpPackage::PNG){
        //clipboard->setImage();
    } else {
        throw;
    }
}

void ClipboardService::getClipboardData()
{
    QMimeData const * data = clipboard->mimeData();
    TcpPackage packageType;
    QByteArray temp;
    if(data->hasText()){
        packageType = TcpPackage::TXT;
        QString text = data->text();
        temp = QByteArray(text.toUtf8().constData(), text.size());

    } else if(data->hasImage()) {
        packageType = TcpPackage::PNG;
        QVariant tempImageVariant = data->imageData();
        QImage image = tempImageVariant.value<QImage>();
        temp = *(toByteArray(image));

    } else if(data->hasUrls()) {
        QList<QUrl> tempUrlList = data->urls();
        temp.append(qint32(tempUrlList.size()));
        for(auto item : tempUrlList){
            temp.append(item.toString().toUtf8());
        }

    } else {
        throw "Unknown MIME format";
    }

    emit clipboardChanged(temp);
}

QByteArray *ClipboardService::toByteArray(QImage & image)
{
    QByteArray * returnArray = new QByteArray();
    QBuffer buffer(returnArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return returnArray;
}

QImage *ClipboardService::fromByteArray(QByteArray & data)
{
    QImage * returnImage = new QImage();
    returnImage->fromData(data);
    return returnImage;
}
