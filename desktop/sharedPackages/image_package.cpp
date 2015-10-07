#include "image_package.h"

ImagePackage::ImagePackage()
{

}

void ImagePackage::write(QTcpSocket *)
{

}

void ImagePackage::read(QDataStream & in)
{
    in >> size;
    image = new char[size];
    in.readRawData(image,size);

    emit gotData(QByteArray(image, size), "image/png");
    emit gotImage(QByteArray(image, size));
}

