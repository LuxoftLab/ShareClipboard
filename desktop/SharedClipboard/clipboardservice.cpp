#include "clipboardservice.h"

ClipboardService::ClipboardService(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged,this, &ClipboardService::getClipboardData);
}

ClipboardService::~ClipboardService(){}

void ClipboardService::updateClipboard(TcpPackage type, QByteArray &data)
{
        if(type == TcpPackage::TXT){
            clipboard->setText(QString(data));
        } else if(type == TcpPackage::PNG){
            clipboard->setImage(*fromByteArray(data));
        } else {
            throw "Unknown MINE-type";
        }
}

void ClipboardService::getClipboardData()
{
    sent = true;
    QMimeData const * data = clipboard->mimeData();
    TcpPackage packageType;
    QByteArray temp;
    if(data->hasText()){
        packageType = TcpPackage::TXT;
        QString text = data->text();
        temp = QByteArray(text.toUtf8().constData(), text.size());
        emit clipboardChanged(packageType, temp);

    } else if(data->hasImage()) {
        packageType = TcpPackage::PNG;
        QVariant tempImageVariant = data->imageData();
        QImage image = tempImageVariant.value<QImage>();
        temp = *(toByteArray(image));
        emit clipboardChanged(packageType, temp);

    } else if(data->hasUrls()) {
        QList<QUrl> tempUrlList = data->urls();
        emit hasUrls(tempUrlList);

    } else {
        throw "Unknown MIME format";
    }


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
